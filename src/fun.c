#include "prelude.h"
#include "vm.h"
#include "isa_vm.h"

int main(int, char **) {
  /*
  U32 code[256] = {
    B_H_(VM_OP_FUNCTION, 2),
    BBH_(VM_OP_I32_IMM_CONST, 0, 13),
    BBH_(VM_OP_I32_IMM_CONST, 1, 42),
    BBBB(VM_OP_I32_ADD, 0, 1, 1),
    BB__(VM_OP_SHOW, 0),
    BB__(VM_OP_SHOW, 1),
    BB__(VM_OP_EXIT, 0),
  };
  */

  U32 code[256] = {
    B_H_(VM_OP_FUNCTION, 0),
    BBH_(VM_OP_I32_IMM_CONST, 0, 9),
    BX__(VM_OP_CALL, 3),
    BB__(VM_OP_SHOW, 0),
    BB__(VM_OP_EXIT, 0),
    B_H_(VM_OP_FUNCTION, 3),
    BBH_(VM_OP_I32_IMM_CONST, 1, 1),
    BBBB(VM_OP_I32_CMP_LE_U, 0, 1, 0),
    BX__(VM_OP_JUMP, 9),
    BBH_(VM_OP_I32_IMM_CONST, 2, 2),
    BBBB(VM_OP_I32_SUB, 0, 2, 3),
    BX__(VM_OP_CALL, (U32) -6),
    BBB_(VM_OP_MOVE, 3, 2),
    BBBB(VM_OP_I32_SUB, 0, 1, 3),
    BX__(VM_OP_CALL, (U32) -9),
    BBBB(VM_OP_I64_ADD, 2, 3, 0),
    BB__(VM_OP_RETURN, 0),
    BB__(VM_OP_RETURN, 0),
  };

  struct VM_State * vm;

  VM_init(&vm);
  VM_exec(vm, code);
  VM_drop(&vm);

  return 0;
}
