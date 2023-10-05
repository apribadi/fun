struct Vm;

void VM_init(struct Vm **);

void VM_drop(struct Vm **);

void VM_exec(struct Vm *, U32 *);
