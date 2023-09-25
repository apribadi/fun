#include "prelude.h"
#include "rts.h"
#include "bvm.h"
#include "code_bvm.h"

typedef enum ExitCode : U32 {
  EXIT_CODE_OK,
  EXIT_CODE_PANIC,
} ExitCode;

typedef ExitCode (* OpHandler)(struct Bvm *, char *, char *, char *);

typedef struct Bvm {
  OpHandler dispatch[BVM_OP_COUNT];
} Bvm;

static inline ExitCode dispatch(struct Bvm * ep, char * ip, char * fp, char * sp) {
  return ep->dispatch[PEEK_L16(ip)](ep, ip, fp, sp);
}

static inline ExitCode op_abort(struct Bvm * ep, char * ip, char * fp, char * sp) {
  (void) ep;
  (void) fp;
  (void) sp;

  return PEEK_L32(ip + 2);
}

void BVM_init(struct Bvm ** p) {
  struct Bvm * ep = RTS_alloc(sizeof(Bvm));

  ep->dispatch[BVM_OP_ABORT] = op_abort;

  * p = ep;
}

void BVM_drop(struct Bvm ** p) {
  free(* p);

  * p = nullptr;
}

void BVM_exec(struct Bvm * ep, char * ip) {
  switch (dispatch(ep, ip, nullptr, nullptr)) {
    case EXIT_CODE_OK:
      printf("ok\n");
      break;
    case EXIT_CODE_PANIC:
      printf("panic!\n");
      break;
  }
}
