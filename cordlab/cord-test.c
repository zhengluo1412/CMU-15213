#include "cord-interface.h"

int main(int argc, char** argv) {
  cord_t c;

  c = cord_new("");
  assert(c == NULL);
  assert(cord_length(c) == 0);

  return 0;
}