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
//
// Rationale:
//
// ???
//
// truncate & zero-extend are canonical
//
// conversions between F32, F64, & Bool are not canonical
//
// don't want to leak reference type representations.

// | Byte 0        | Byte 1        | Byte 2        | Byte 3       |
// |               |               |               |              |
// | OP_CALL       | displacement (24 bit signed)                 |
// | OP_EXIT       | exit code     |               |              |
// | OP_FUNCTION   |               | frame size (16 bit unsigned) |
// | OP_JUMP       | displacement (24 bit signed)                 |
// | OP_MOVE       | src           | dst           |              |
// | OP_RETURN     | kont index    |               |              |
// |               |               |               |              |
// | OP_ADD        | src           | src           | dst          |
// | OP_IMM_CONST  | dst           | immediate (16 bit signed)    |
//
typedef enum VM_OpCode : U8 {
  VM_OP_CALL,
  VM_OP_EXIT,
  VM_OP_FUNCTION,
  VM_OP_JUMP,
  VM_OP_MOVE,
  VM_OP_RETURN,
  VM_OP_SHOW,
  VM_OP_F32_ADD,
  VM_OP_F64_ADD,
  VM_OP_I32_ADD,
  VM_OP_I32_CMP_LE_U,
  VM_OP_I32_IMM_CONST,
  VM_OP_I32_SUB,
  VM_OP_I64_ADD,
  VM_OP_COUNT,
} VM_OpCode;
