#include "prelude.h"
#include "rts.h"
#include "bvm.h"
#include "isa_bvm.h"

typedef enum ExitCode : S8 {
  EXIT_CODE_OK,
  EXIT_CODE_PANIC,
} ExitCode;

typedef ExitCode (* OpHandler)(struct Bvm *, U32 *, U64 *, U64 *, U64);

typedef struct Bvm {
  OpHandler dispatch[BVM_OP_COUNT];
} Bvm;

static inline F32 GET_REG_F32(U64 * sp, U16 i) {
  U64 x = sp[i];
  F64 y;
  memcpy(&y, &x, sizeof(F64));
  return (F32) y;
}

static inline F64 GET_REG_F64(U64 * sp, U16 i) {
  U64 x = sp[i];
  F64 y;
  memcpy(&y, &x, sizeof(F64));
  return y;
}

static inline U32 GET_REG_U32(U64 * sp, U16 i) {
  return (U32) sp[i];
}

static inline U64 GET_REG_U64(U64 * sp, U16 i) {
  return sp[i];
}

static inline void SET_REG_F32(U64 * sp, U16 i, F32 x) {
  F64 y = (F64) x;
  U64 z;
  memcpy(&z, &y, sizeof(F64));
  sp[i] = z;
}

static inline void SET_REG_F64(U64 * sp, U16 i, F64 x) {
  U64 y;
  memcpy(&y, &x, sizeof(F64));
  sp[i] = y;
}

static inline void SET_REG_U32(U64 * sp, U16 i, U32 x) {
  sp[i] = x;
}

static inline void SET_REG_U64(U64 * sp, U16 i, U64 x) {
  sp[i] = x;
}

static inline ExitCode dispatch(Bvm * ep, U32 * ip, U64 * sp, U64 * lp) {
  U32 inst = * ip;
  return ep->dispatch[B0(inst)](ep, ip, sp, lp, inst);
}

static ExitCode op_abort(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  (void) ep;
  (void) ip;
  (void) sp;
  (void) lp;

  return (S8) B2(inst);
}

static ExitCode op_call(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  lp[0] = (U64) sp;
  lp[1] = (U64) (ip + 1);

  // ???
  S32 k = (S32) W1(inst);

  return dispatch(ep, ip + k, sp, lp + 2);
}

static ExitCode op_debug_print(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  U64 x = sp[B1(inst)];

  printf("%" PRIx64 "\n", x);

  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_frame(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  // TODO: stack overflow check w/ red zone
  // TODO: copy arguments into frame

  sp = sp - H1(inst);

  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_goto(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  S16 k = (S16) H1(inst);

  return dispatch(ep, ip + k, sp, lp);
}

static ExitCode op_nop(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  (void) inst;

  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_return(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  (void) inst;

  sp = (U64 *) lp[0];
  ip = (U32 *) lp[1];
  lp = lp - 2;

  return dispatch(ep, ip, sp, lp);
}

static ExitCode op_f32_add(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  F32 x = GET_REG_F32(sp, B1(inst));
  F32 y = GET_REG_F32(sp, B2(inst));
  F32 z = x + y;
  SET_REG_F32(sp, B3(inst), z);
  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_f64_add(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  F64 x = GET_REG_F64(sp, B1(inst));
  F64 y = GET_REG_F64(sp, B2(inst));
  F64 z = x + y;
  SET_REG_F64(sp, B3(inst), z);
  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_i32_add(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  U32 x = GET_REG_U32(sp, B1(inst));
  U32 y = GET_REG_U32(sp, B2(inst));
  U32 z = x + y;
  SET_REG_U32(sp, B3(inst), z);
  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_i64_add(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  U64 x = GET_REG_U64(sp, B1(inst));
  U64 y = GET_REG_U64(sp, B2(inst));
  U64 z = x + y;
  SET_REG_U64(sp, B3(inst), z);
  return dispatch(ep, ip + 1, sp, lp);
}

void BVM_init(struct Bvm ** p) {
  Bvm * ep = RTS_alloc(sizeof(Bvm));

  * ep = (Bvm) {
    .dispatch = {
      [BVM_OP_ABORT] = op_abort,
      [BVM_OP_CALL] = op_call,
      [BVM_OP_DEBUG_PRINT] = op_debug_print,
      [BVM_OP_FRAME] = op_frame,
      [BVM_OP_GOTO] = op_goto,
      [BVM_OP_NOP] = op_nop,
      [BVM_OP_RETURN] = op_return,
      [BVM_OP_F32_ADD] = op_f32_add,
      [BVM_OP_F64_ADD] = op_f64_add,
      [BVM_OP_I32_ADD] = op_i32_add,
      [BVM_OP_I64_ADD] = op_i64_add,
    },
  };

  * p = ep;
}

void BVM_drop(struct Bvm ** p) {
  RTS_free(* p);

  * p = nullptr;
}

void BVM_exec(struct Bvm * ep, U32 * ip) {
  switch (dispatch(ep, ip, nullptr, nullptr)) {
    case EXIT_CODE_OK:
      printf("ok\n");
      break;
    case EXIT_CODE_PANIC:
      printf("panic!\n");
      break;
  }
}
