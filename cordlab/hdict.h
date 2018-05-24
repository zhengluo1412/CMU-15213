#ifndef HDICT_H
#define HDICT_H

/************************/
/*** Client interface ***/
/************************/

// typedef _______ key;
typedef void* key;
// typedef ______* value;
typedef void* value;

typedef bool key_equiv_fn(key x, key y);
typedef int key_hash_fn(key x);

/*************************/
/*** Library interface ***/
/*************************/

// typedef ______* hdict_t;
typedef struct hdict_header* hdict_t;
typedef struct chain_node chain;
struct chain_node {
  key k;
  value v;
  chain* next;
};

typedef struct hdict_header hdict;
struct hdict_header {
  int size;
  int capacity;               /* 0 < capacity */
  chain** table;             /* \length(table) == capacity */
  key_equiv_fn* equiv;        /* non-NULL */
  key_hash_fn* hash;          /* non-NULL */
};

hdict* hdict_new(int capacity, key_equiv_fn* equiv, key_hash_fn* hash)
  /*@requires capacity > 0 && equiv != NULL && hash != NULL; @*/
  /*@ensures \result != NULL; @*/ ;

value hdict_lookup(hdict_t H, key k)
  /*@requires H != NULL; @*/ ;

void hdict_insert(hdict_t H, key k, value v)
  /*@requires H != NULL && v != NULL; @*/
  /*@ensures hdict_lookup(H, k) == v; @*/ ;

#endif