#include "prelude.h"
#include "bvm.h"
#include "isa_bvm.h"

int main(int, char **) {
  U32 code[1024];

  struct Bvm * vm;

  code[0] = BBBB(BVM_OP_EXIT, 0, 1, 0);

  BVM_init(&vm);
  BVM_exec(vm, code);
  BVM_drop(&vm);

  return 0;
}
