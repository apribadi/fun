#include "prelude.h"
#include "rts.h"
#include "bvm.h"
#include "code_bvm.h"

typedef enum ExitCode : U32 {
  EXIT_CODE_OK,
  EXIT_CODE_PANIC,
} ExitCode;

typedef ExitCode (* OpHandler)(struct Bvm *, U64 *, U64 *, U64 *, U64);

typedef struct Bvm {
  OpHandler dispatch[BVM_OP_COUNT];
} Bvm;

static inline F32 GET_REG_F32(U64 * fp, U16 i) {
  U32 x = (U32) fp[i];
  F32 y;
  memcpy(&y, &x, sizeof(F32));
  return y;
}

static inline F64 GET_REG_F64(U64 * fp, U16 i) {
  U64 x = fp[i];
  F64 y;
  memcpy(&y, &x, sizeof(F64));
  return y;
}

static inline U32 GET_REG_U32(U64 * fp, U16 i) {
  return (U32) fp[i];
}

static inline U64 GET_REG_U64(U64 * fp, U16 i) {
  return fp[i];
}

static inline void SET_REG_F32(U64 * fp, U16 i, F32 x) {
  U32 y;
  memcpy(&y, &x, sizeof(F32));
  fp[i] = y;
}

static inline void SET_REG_F64(U64 * fp, U16 i, F64 x) {
  U64 y;
  memcpy(&y, &x, sizeof(F64));
  fp[i] = y;
}

static inline void SET_REG_U32(U64 * fp, U16 i, U32 x) {
  fp[i] = x;
}

static inline void SET_REG_U64(U64 * fp, U16 i, U64 x) {
  fp[i] = x;
}

static inline ExitCode dispatch(Bvm * ep, U64 * ip, U64 * fp, U64 * sp) {
  U64 inst = * ip;
  return ep->dispatch[H0(inst)](ep, ip, fp, sp, inst);
}

static ExitCode op_abort(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  (void) ep;
  (void) ip;
  (void) fp;
  (void) sp;

  return W1(inst);
}

static ExitCode op_call(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  sp[0] = (U64) fp;
  sp[1] = (U64) (ip + 1);

  S32 k = (S32) W1(inst);

  return dispatch(ep, ip + k, fp, sp);
}

static ExitCode op_debug_print(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  U64 x = fp[H1(inst)];

  printf("%" PRIx64 "\n", x);

  return dispatch(ep, ip + 1, fp, sp);
}

static ExitCode op_frame(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  // TODO: stack overflow check

  fp = sp + 2;
  sp = sp + 2 + H2(inst);

  return dispatch(ep, ip + 1, fp, sp);
}

static ExitCode op_jump(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  S32 k = (S32) W1(inst);

  return dispatch(ep, ip + k, fp, sp);
}

static ExitCode op_nop(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  (void) inst;

  return dispatch(ep, ip + 1, fp, sp);
}

static ExitCode op_f32_add(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  F32 x = GET_REG_F32(fp, H1(inst));
  F32 y = GET_REG_F32(fp, H2(inst));
  F32 z = x + y;
  SET_REG_F32(fp, H3(inst), z);
  return dispatch(ep, ip + 1, fp, sp);
}

static ExitCode op_f64_add(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  F64 x = GET_REG_F64(fp, H1(inst));
  F64 y = GET_REG_F64(fp, H2(inst));
  F64 z = x + y;
  SET_REG_F64(fp, H3(inst), z);
  return dispatch(ep, ip + 1, fp, sp);
}

static ExitCode op_i32_add(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  U32 x = GET_REG_U32(fp, H1(inst));
  U32 y = GET_REG_U32(fp, H2(inst));
  U32 z = x + y;
  SET_REG_U32(fp, H3(inst), z);
  return dispatch(ep, ip + 1, fp, sp);
}

static ExitCode op_i64_add(Bvm * ep, U64 * ip, U64 * fp, U64 * sp, U64 inst) {
  U64 x = GET_REG_U64(fp, H1(inst));
  U64 y = GET_REG_U64(fp, H2(inst));
  U64 z = x + y;
  SET_REG_U64(fp, H3(inst), z);
  return dispatch(ep, ip + 1, fp, sp);
}

void BVM_init(struct Bvm ** p) {
  Bvm * ep = RTS_alloc(sizeof(Bvm));

  * ep = (Bvm) {
    .dispatch = {
      [BVM_OP_ABORT] = op_abort,
      [BVM_OP_CALL] = op_call,
      [BVM_OP_DEBUG_PRINT] = op_debug_print,
      [BVM_OP_FRAME] = op_frame,
      [BVM_OP_JUMP] = op_jump,
      [BVM_OP_NOP] = op_nop,
      [BVM_OP_F32_ADD] = op_f32_add,
      [BVM_OP_F64_ADD] = op_f64_add,
      [BVM_OP_I32_ADD] = op_i32_add,
      [BVM_OP_I64_ADD] = op_i64_add,
    },
  };

  * p = ep;
}

void BVM_drop(struct Bvm ** p) {
  free(* p);

  * p = nullptr;
}

void BVM_exec(struct Bvm * ep, U64 * ip) {
  switch (dispatch(ep, ip, nullptr, nullptr)) {
    case EXIT_CODE_OK:
      printf("ok\n");
      break;
    case EXIT_CODE_PANIC:
      printf("panic!\n");
      break;
  }
}
