#include "prelude.h"
#include "rts.h"
#include "bvm.h"
#include "isa_bvm.h"

//            |           |
//            |           |
//            | arg/ret 2 | ----+
//            | arg/ret 1 |     |
//            | arg/ret 0 |     |
// +--------> +-----------+     +--- register file
// |          |   local   |     |
// |          |   local   |     |
// |          |   local   | ----+
// |  sp ---> +-----------+
// |          |           |
// |          |           |
// |          |           |
// |          |           |
// |          |           |
// |  lp ---> +-----------+
// +----------|--prev sp  |
//            |  prev ip  |
//            +-----------+
//            |           |
//            |           |

typedef enum ExitCode : U8 {
  EXIT_CODE_OK,
  EXIT_CODE_PANIC,
} ExitCode;

typedef ExitCode (* OpHandler)(struct Bvm *, U32 *, U64 *, U64 *, U64);

typedef struct Bvm {
  OpHandler dispatch[BVM_OP_COUNT];
  U64 buf[256];
} Bvm;

static inline F32 get_reg_f32(U64 * p) {
  F64 x;
  memcpy(&x, p, sizeof(F64));
  return (F32) x;
}

static inline F64 get_reg_f64(U64 * p) {
  F64 x;
  memcpy(&x, p, sizeof(F64));
  return x;
}

static inline U32 get_reg_u32(U64 * p) {
  return (U32) * p;
}

static inline U64 get_reg_u64(U64 * p) {
  return * p;
}

static inline void * get_reg_ptr(U64 * p) {
  return (void *) * p;
}

static inline void set_reg_f32(U64 * p, F32 x) {
  F64 y = (F64) x;
  memcpy(p, &y, sizeof(F64));
}

static inline void set_reg_f64(U64 * p, F64 x) {
  memcpy(p, &x, sizeof(F64));
}

static inline void set_reg_u32(U64 * p, U32 x) {
  * p = x;
}

static inline void set_reg_u64(U64 * p, U64 x) {
  * p = x;
}

static inline void set_reg_ptr(U64 * p, void * x) {
  * p = (U64) x;
}

static inline ExitCode dispatch(Bvm * ep, U32 * ip, U64 * sp, U64 * lp) {
  U32 inst = * ip;
  return ep->dispatch[B0(inst)](ep, ip, sp, lp, inst);
}

// BVM_OP_CALL | arity | fnptr & args | ---

static ExitCode op_call(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  set_reg_ptr(lp, sp);
  set_reg_ptr(lp + 1, ip + 1);
  lp = lp + 2;

  Int n = B1(inst);
  Int i = B2(inst);

  ip = get_reg_ptr(sp + i);

  i ++;

  for (Int j = 0; j < n; j ++) ep->buf[j] = sp[i ++];

  return dispatch(ep, ip, sp, lp);
}

static ExitCode op_debug_print(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  U64 x = sp[B1(inst)];

  printf("%" PRIx64 "\n", x);

  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_exit(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  (void) ep;
  (void) ip;
  (void) sp;
  (void) lp;

  return B2(inst);
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

  lp = lp - 2;
  sp = get_reg_ptr(lp);
  ip = get_reg_ptr(lp + 1);

  return dispatch(ep, ip, sp, lp);
}

static ExitCode op_f32_add(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  F32 x = get_reg_f32(sp + B1(inst));
  F32 y = get_reg_f32(sp + B2(inst));
  F32 z = x + y;
  set_reg_f32(sp + B3(inst), z);
  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_f64_add(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  F64 x = get_reg_f64(sp + B1(inst));
  F64 y = get_reg_f64(sp + B2(inst));
  F64 z = x + y;
  set_reg_f64(sp + B3(inst), z);
  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_i32_add(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  U32 x = get_reg_u32(sp + B1(inst));
  U32 y = get_reg_u32(sp + B2(inst));
  U32 z = x + y;
  set_reg_u32(sp + B3(inst), z);
  return dispatch(ep, ip + 1, sp, lp);
}

static ExitCode op_i64_add(Bvm * ep, U32 * ip, U64 * sp, U64 * lp, U64 inst) {
  U64 x = get_reg_u64(sp + B1(inst));
  U64 y = get_reg_u64(sp + B2(inst));
  U64 z = x + y;
  set_reg_u64(sp + B3(inst), z);
  return dispatch(ep, ip + 1, sp, lp);
}

void BVM_init(struct Bvm ** p) {
  Bvm * ep = RTS_alloc(sizeof(Bvm));

  * ep = (Bvm) {
    .dispatch = {
      [BVM_OP_EXIT] = op_exit,
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
