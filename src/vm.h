struct VM_State;

void VM_init(struct VM_State **);

void VM_drop(struct VM_State **);

void VM_exec(struct VM_State *, U32 *);
