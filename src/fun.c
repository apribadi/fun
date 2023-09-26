#include "prelude.h"
#include "bvm.h"
#include "code_bvm.h"

int main(int, char **) {
  U64 code[1024];

  struct Bvm * vm;

  code[0] = H_W_(BVM_OP_ABORT, 1);

  BVM_init(&vm);
  BVM_exec(vm, code);
  BVM_drop(&vm);

  return 0;
}
