struct Bvm;

void BVM_init(struct Bvm **);

void BVM_drop(struct Bvm **);

void BVM_exec(struct Bvm *, char *);
