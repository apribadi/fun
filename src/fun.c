#include "prelude.h"
#include "vm.h"
#include "isa_vm.h"

int main(int, char **) {
  U32 code[256] = {
    B_H_(VM_OP_FUNCTION, 2),
    BBH_(VM_OP_I32_IMM_CONST, 0, 13),
    BBH_(VM_OP_I32_IMM_CONST, 1, 42),
    BBBB(VM_OP_I32_ADD, 0, 1, 1),
    BB__(VM_OP_SHOW, 0),
    BB__(VM_OP_SHOW, 1),
    BB__(VM_OP_EXIT, 0),
  };

  struct VM_State * vm;

  VM_init(&vm);
  VM_exec(vm, code);
  VM_drop(&vm);

  return 0;
}
