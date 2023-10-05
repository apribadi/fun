#include "prelude.h"
#include "rt.h"
#include "vm.h"
#include "isa_vm.h"

//         |           |
//         |           |
//         +-----------+
//         |  prev ip  |
//         |  prev fp--|----------------------+
// lp ---> +-----------+                      |
//         |           |                      |
//         |           |                      |
//         |           |                      |
//         |           |                      |
//         | arg/ret 2 | <---+                |
//         | arg/ret 1 |     |                |
//         | arg/ret 0 |     |                |
// sp ---> +-----------+     |                |
//         |   local   |     |                |
//         |   local   |     +--- registers   |
//         |   local   |     |                |
//         |   local   |     |                |
//         |   local   |     |                |
//         | arg/ret 1 |     |                |
//         | arg/ret 0 | <---+                |
// fp ---> +-----------+                      |
//         |           |                      |
//         |           |                      |
//         |           |                      |
//         +-----------+ <--------------------+
//         |           |
//         |           |
//         |           |

typedef enum ExitCode : U8 {
  EXIT_CODE_OK,
  EXIT_CODE_PANIC,
  EXIT_CODE_STACK_OVERFLOW,
} ExitCode;

typedef ExitCode (* OpHandler)(struct VM_State *, U32 *, U64 *, U64 *, U64 *, U32);

typedef struct VM_State {
  OpHandler dispatch[VM_OP_COUNT];
} VM_State;

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

static inline ExitCode dispatch(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp) {
  U32 inst = * ip;
  return ep->dispatch[B0(inst)](ep, ip, fp, sp, lp, inst);
}

static ExitCode op_call(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  set_reg_ptr(lp - 2, fp);
  set_reg_ptr(lp - 1, ip + 1);

  S32 k = (S32) inst >> 8;

  return dispatch(ep, ip + k, sp, sp, lp - 2);
}

static ExitCode op_exit(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  (void) ep;
  (void) ip;
  (void) fp;
  (void) sp;
  (void) lp;

  return B1(inst);
}

static ExitCode op_function(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  if (! (258 <= lp - sp)) return EXIT_CODE_STACK_OVERFLOW;

  U16 k = H1(inst);

  return dispatch(ep, ip + 1, fp, sp + k, lp);
}

static ExitCode op_jump(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  S32 k = (S32) inst >> 8;

  return dispatch(ep, ip + k, fp, sp, lp);
}

static ExitCode op_move(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  fp[B2(inst)] = fp[B1(inst)];

  return dispatch(ep, ip + 1, fp, sp, lp);
}

static ExitCode op_return(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  (void) ip;
  (void) sp;

  U64 * x = get_reg_ptr(lp);
  U32 * y = (U32 *) get_reg_ptr(lp + 1) + B1(inst);

  return dispatch(ep, y, x, fp, lp + 2);
}

static ExitCode op_show(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  U64 x = fp[B1(inst)];

  printf("%" PRId64 "\n", x);

  return dispatch(ep, ip + 1, fp, sp, lp);
}

static ExitCode op_f32_add(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  F32 x = get_reg_f32(fp + B1(inst));
  F32 y = get_reg_f32(fp + B2(inst));
  F32 z = x + y;
  set_reg_f32(fp + B3(inst), z);
  return dispatch(ep, ip + 1, fp, sp, lp);
}

static ExitCode op_f64_add(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  F64 x = get_reg_f64(fp + B1(inst));
  F64 y = get_reg_f64(fp + B2(inst));
  F64 z = x + y;
  set_reg_f64(fp + B3(inst), z);
  return dispatch(ep, ip + 1, fp, sp, lp);
}

static ExitCode op_i32_add(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  U32 x = get_reg_u32(fp + B1(inst));
  U32 y = get_reg_u32(fp + B2(inst));
  U32 z = x + y;
  set_reg_u32(fp + B3(inst), z);
  return dispatch(ep, ip + 1, fp, sp, lp);
}

static ExitCode op_i32_cmp_le_u(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  U32 x = get_reg_u32(fp + B1(inst));
  U32 y = get_reg_u32(fp + B2(inst));
  bool p = x <= y;
  bool q = B3(inst);
  return dispatch(ep, ip + 1 + (p == q), fp, sp, lp);
}

static ExitCode op_i32_imm_const(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  U32 x = (U32) (S32) (S16) H1(inst);
  set_reg_u32(fp + B1(inst), x);
  return dispatch(ep, ip + 1, fp, sp, lp);
}

static ExitCode op_i32_sub(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  U32 x = get_reg_u32(fp + B1(inst));
  U32 y = get_reg_u32(fp + B2(inst));
  U32 z = x - y;
  set_reg_u32(fp + B3(inst), z);
  return dispatch(ep, ip + 1, fp, sp, lp);
}

static ExitCode op_i64_add(VM_State * ep, U32 * ip, U64 * fp, U64 * sp, U64 * lp, U32 inst) {
  U64 x = get_reg_u64(fp + B1(inst));
  U64 y = get_reg_u64(fp + B2(inst));
  U64 z = x + y;
  set_reg_u64(fp + B3(inst), z);
  return dispatch(ep, ip + 1, fp, sp, lp);
}

static VM_State STATE = {
  .dispatch = {
    [VM_OP_CALL] = op_call,
    [VM_OP_EXIT] = op_exit,
    [VM_OP_FUNCTION] = op_function,
    [VM_OP_JUMP] = op_jump,
    [VM_OP_MOVE] = op_move,
    [VM_OP_RETURN] = op_return,
    [VM_OP_SHOW] = op_show,
    [VM_OP_F32_ADD] = op_f32_add,
    [VM_OP_F64_ADD] = op_f64_add,
    [VM_OP_I32_ADD] = op_i32_add,
    [VM_OP_I32_CMP_LE_U] = op_i32_cmp_le_u,
    [VM_OP_I32_IMM_CONST] = op_i32_imm_const,
    [VM_OP_I32_SUB] = op_i32_sub,
    [VM_OP_I64_ADD] = op_i64_add,
  },
};

void VM_init(struct VM_State ** p) {
  * p = &STATE;
}

void VM_drop(struct VM_State ** p) {
  * p = nullptr;
}

void VM_exec(struct VM_State * ep, U32 * ip) {
  U64 stack[1024];

  U64 * fp = stack;
  U64 * sp = fp;
  U64 * lp = fp + 1024;

  switch (dispatch(ep, ip, fp, sp, lp)) {
    case EXIT_CODE_OK:
      printf("ok\n");
      break;
    case EXIT_CODE_PANIC:
      printf("panic!\n");
      break;
    case EXIT_CODE_STACK_OVERFLOW:
      printf("stack overflow!\n");
      break;
  }
}
