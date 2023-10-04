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

// | Byte 0      | Byte 1      | Byte 2      | Byte 3      |
// |             |             |             |             |
// | OP_ARGS     | # args      | reg start   |             |
// | OP_CALL     | signed displacement (24 bit)            |
// | OP_EXIT     | exit code   |             |             |
// | OP_FUNC     | # args      | frame size (16 bit)       |
// | OP_JUMP     | signed displacement (24 bit)            |
// | OP_RETN     | kont index  |             |             |
// | OP_TAIL     | signed displacement (24 bit)            |
// |             |             |             |             |
// | OP_MOVE     | src         | dst         |             |
// | OP_SPILL    | src         | dst (16 bit)              |
// | OP_RELOAD   | src (16 bit)              | dst         |
//
//
typedef enum BVM_OpCode : U8 {
  BVM_OP_CALL,
  BVM_OP_DEBUG_PRINT,
  BVM_OP_EXIT,
  BVM_OP_FRAME,
  BVM_OP_GOTO,
  BVM_OP_NOP,
  BVM_OP_RETURN,
  BVM_OP_F32_ADD,
  BVM_OP_F64_ADD,
  BVM_OP_I32_ADD,
  BVM_OP_I64_ADD,
  BVM_OP_COUNT,
} BVM_OpCode;
