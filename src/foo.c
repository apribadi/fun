#include "prelude.h"

U16 foo(char * p) {
  U16 x = POP_LE16(p);
  U16 y = POP_LE16(p);
  U16 z = POP_LE16(p);
  return x + y + z;
}
