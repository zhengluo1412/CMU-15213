/*******************/
/* Cords interface */
/*******************/

#ifndef CORD_INTERFACE_H
#define CORD_INTERFACE_H
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

// typedef ______________ cord_t;
typedef struct cord_node* cord_t;
typedef struct cord_node cord;
struct cord_node {
  int len;
  cord* left;
  cord* right;
  char* data;
};

int    cord_length(cord_t R);
cord_t cord_new(char* s);
cord_t cord_join(cord_t R, cord_t S);
char* cord_tostring(cord_t R);
char   cord_charat(cord_t R, int i) 
  /*@requires 0 <= i && i < cord_length(R); @*/ ;
cord_t cord_sub(cord_t R, int lo, int hi)
  /*@requires 0 <= lo && lo <= hi && hi <= cord_length(R); @*/ ;
void   cord_reduce(int n, cord_t A[n])
  /*@requires n == \length(A); @*/ ;
bool is_cord(cord*);

#endif
