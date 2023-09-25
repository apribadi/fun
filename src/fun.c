#include "prelude.h"
#include "bvm.h"
#include "code_bvm.h"

int main(int, char **) {
  char code[1024];

  struct Bvm * vm;

  POKE_L16(&code[0], BVM_OP_ABORT);
  POKE_L32(&code[2], 1);

  BVM_init(&vm);
  BVM_exec(vm, code);
  BVM_drop(&vm);

  return 0;
}
