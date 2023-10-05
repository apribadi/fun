#include "prelude.h"
#include "vm.h"
#include "isa_vm.h"

int main(int, char **) {
  U32 code[1024];

  struct Vm * vm;

  code[0] = BBBB(VM_OP_EXIT, 0, 1, 0);

  VM_init(&vm);
  VM_exec(vm, code);
  VM_drop(&vm);

  return 0;
}
