/* 
 * Resizing hash sets, implemented with separate chaining
 * 
 * 15-513
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hdict.h"

/**********************/
/*** Implementation ***/
/**********************/

bool keyequiv(hdict* H, key x, key y)
//@requires H != NULL && H->equiv != NULL;
{
  return (*H->equiv)(x, y);
}

int keyhash(hdict* H, key x) 
//@requires H != NULL && H->capacity > 0 && H->hash != NULL;
//@ensures 0 <= \result && \result < H->capacity;
{
  return abs((*H->hash)(x) % H->capacity);
}

// All hash values are correct for the current index
bool is_chain(hdict* H, chain* p, int i) 
//@requires H != NULL && H->capacity > 0 && H->hash != NULL;
{
  if (p == NULL) return true;
  return p->v != NULL 
    && keyhash(H, p->k) == i
    && is_chain(H, p->next, i);
}

bool is_chains(hdict* H, int lo, int hi) 
//@requires H != NULL && H->capacity > 0 && H->hash != NULL;
//@requires \length(H->table) == H->capacity;
//@requires 0 <= lo && lo <= hi && hi <= H->capacity;
{
  if (lo == hi) return true;
  return is_chain(H, H->table[lo], lo) 
    && is_chains(H, lo+1, hi);
}

bool is_table_expected_length(chain** table, int length) {
  //@assert \length(table) == length;
  return true;
}

bool is_hdict(hdict* H) {
  return H != NULL
    && H->capacity > 0
    && H->size >= 0 
    && H->equiv != NULL
    && H->hash != NULL
    && is_table_expected_length(H->table, H->capacity)
    && is_chains(H, 0, H->capacity);
}

hdict* hdict_new(int capacity, key_equiv_fn* equiv, key_hash_fn* hash)
//@requires capacity > 0 && equiv != NULL && hash != NULL;
//@ensures is_hdict(\result);
{
  hdict* H = malloc(sizeof(hdict));
  H->size = 0;
  H->capacity = capacity;
  H->table = malloc(sizeof(chain*) * capacity);
  H->equiv = equiv;
  H->hash = hash;
  return H;
}

value hdict_lookup(hdict* H, key k)
//@requires is_hdict(H);
{
  int i = keyhash(H, k);
  for (chain* p = H->table[i]; p != NULL; p = p->next)
  //@loop_invariant is_chain(H, p, i);
  {
    if (keyequiv(H, p->k, k)) {
      return p->v;
    }
  }

  return NULL;
}

void hdict_resize(hdict* H) 
//@requires is_hdict(H);
//@requires H->size >= H->capacity;
//@ensures is_hdict(H);
{
  int old_capacity = H->capacity;
  int new_capacity = H->size * 2;
  assert(new_capacity > H->capacity);
  chain** old_table = H->table;
  chain** new_table = malloc(sizeof(chain*) * new_capacity);

  H->capacity = new_capacity; // is_hdict now violated!
  H->table = new_table; // is_hdict restored *only* because we don't check size

  for (int i = 0; i < old_capacity; i++) 
  //@loop_invariant 0 <= i && i <= old_capacity;
  //@loop_invariant is_hdict(H); // only because we don't check size
  {
    chain* p = old_table[i];
    while (p != NULL)
    //@loop_invariant is_hdict(H); // only because we don't check size
    {
      //@assert p->v != NULL;
      chain* q = p->next; // Save the rest of the chain
      int h = keyhash(H, p->k);
      p->next = H->table[h];
      H->table[h] = p;
      p = q;
    }
  }
}

void hdict_insert(hdict* H, key k, value v) 
//@requires is_hdict(H);
//@requires v != NULL;
//@ensures is_hdict(H);
//@ensures v == hdict_lookup(H, k);
{
  int i = keyhash(H, k);
  for (chain* p = H->table[i]; p != NULL; p = p->next)
  //@loop_invariant is_chain(H, p, i);
  {
    if (keyequiv(H, p->k, k)) {
      p->v = v;
      return;
    }
  }
  
  // prepend new element
  chain* p = malloc(sizeof(chain));
  p->k = k;
  p->v = v;
  p->next = H->table[i];
  H->table[i] = p;
  (H->size)++;
  assert(H->size > 0);

  if (H->size >= H->capacity) hdict_resize(H); 
}
