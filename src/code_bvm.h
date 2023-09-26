// In the virtual machine, values in registers have one of six types:
//
// - Bool
// - F32
// - F64
// - Int
// - Ptr
// - V256
//
// Notably, all of the SSA integer types
//
// - I5
// - I6
// - I8
// - I32
// - I64
//
// are collapsed into a single Int type. Primitive operations that nominally
// work with the narrower types truncate their inputs and zero-extend their
// outputs.

typedef enum BVM_OpCode : U16 {
  BVM_OP_ABORT,
  BVM_OP_CALL,
  BVM_OP_DEBUG_PRINT,
  BVM_OP_FRAME,
  BVM_OP_JUMP,
  BVM_OP_NOP,
  BVM_OP_F32_ADD,
  BVM_OP_F64_ADD,
  BVM_OP_I32_ADD,
  BVM_OP_I64_ADD,
  BVM_OP_COUNT,
} BVM_OpCode;
