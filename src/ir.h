typedef enum IR_Code : U16 {
  IR_CALL,     // n_args U16, n_konts U16, disp U32, arg_0 U16, ..., kont_0 U32, ...
  IR_FUNC,     // n_args U16, n_konts U16, type_0 U16, ...
  IR_GOTO,     // n_args U16, disp U32, arg_0 U16, ...
  IR_INST_11_, // prim U16, arg_0 U16
  IR_INST_1_0, // prim U16, arg_0 U16
  IR_INST_1_2, // prim U16, arg_0 U16, disp_0 U32, disp_1 U32
  IR_INST_20_, // prim U16, arg_0 U16, arg_1 U16
  IR_INST_21_, // prim U16, arg_0 U16, arg_1 U16
  IR_INST_2_2, // prim U16, arg_0 U16, arg_1 U16, disp_0 U32, disp_1 U32
  IR_INST_30_, // prim U16, arg_0 U16, arg_1 U16, arg_2 U16
  IR_INST_31_, // prim U16, arg_0 U16, arg_1 U16, arg_2 U16
  IR_LABEL,    // n_args U16, type_0 U16, ...
  IR_RETURN,   // n_args U16, kont_index U16, arg_0 U16, ...
  IR_TAIL      // n_args U16, disp U32, arg_0 U16, ...
} IR_Code;

typedef enum IR_OpCode : U16 {
  IR_OP_CALL,
  IR_OP_CALL_INDIRECT,
  IR_OP_CASE,
  IR_OP_EXIT,
  IR_OP_IF,
  IR_OP_JUMP,
  IR_OP_LABEL,
  IR_OP_NOP,
  IR_OP_RETURN,
  IR_OP_TAIL,
  IR_OP_TAIL_INDIRECT,
  IR_OP_SELECT,
  IR_OP_CONST_F64,
  IR_OP_CONST_I32,
  IR_OP_CONST_I5,
  IR_OP_CONST_I6,
  IR_OP_CONST_I64,
  IR_OP_CONST_V256,
  IR_OP_BOOL_AND,
  IR_OP_BOOL_CONST,
  IR_OP_BOOL_IS_EQ,
  IR_OP_BOOL_IS_LE,
  IR_OP_BOOL_IS_LT,
  IR_OP_BOOL_NOT,
  IR_OP_BOOL_OR,
  IR_OP_BOOL_XOR,
  IR_OP_F32_ABS,
  IR_OP_F32_ADD,
  IR_OP_F32_CONST,
  IR_OP_F32_DIV,
  IR_OP_F32_IS_EQ,
  IR_OP_F32_IS_LE,
  IR_OP_F32_IS_LT,
  IR_OP_F32_MUL,
  IR_OP_F32_ROUND,
  IR_OP_F32_ROUND_I32,
  IR_OP_F32_ROUND_I64,
  IR_OP_F32_SQRT,
  IR_OP_F32_SUB,
  IR_OP_F32_TO_F64,
  IR_OP_F64_ABS,
  IR_OP_F64_ADD,
  IR_OP_F64_CONST,
  IR_OP_F64_DIV,
  IR_OP_F64_IS_EQ,
  IR_OP_F64_IS_LE,
  IR_OP_F64_IS_LT,
  IR_OP_F64_MUL,
  IR_OP_F64_ROUND,
  IR_OP_F64_ROUND_32,
  IR_OP_F64_ROUND_64,
  IR_OP_F64_SQRT,
  IR_OP_F64_SUB,
  IR_OP_F64_TO_F32,
  IR_OP_I32_ADD,
  IR_OP_I32_BIT_AND,
  IR_OP_I32_BIT_NOT,
  IR_OP_I32_BIT_OR,
  IR_OP_I32_BIT_XOR,
  IR_OP_I32_CLZ,
  IR_OP_I32_CONST,
  IR_OP_I32_CTZ,
  IR_OP_I32_IS_EQ,
  IR_OP_I32_IS_LE_S,
  IR_OP_I32_IS_LE_U,
  IR_OP_I32_IS_LT_S,
  IR_OP_I32_IS_LT_U,
  IR_OP_I32_MUL,
  IR_OP_I32_MUL_FULL_S,
  IR_OP_I32_MUL_FULL_U,
  IR_OP_I32_NEG,
  IR_OP_I32_REV,
  IR_OP_I32_ROL,
  IR_OP_I32_ROR,
  IR_OP_I32_SHL,
  IR_OP_I32_SHR_S,
  IR_OP_I32_SHR_U,
  IR_OP_I32_SUB,
  IR_OP_I32_TO_I5,
  IR_OP_I32_TO_I6,
  IR_OP_I32_TO_I64_S,
  IR_OP_I32_TO_I64_U,
  IR_OP_I64_ADD,
  IR_OP_I64_BIT_AND,
  IR_OP_I64_BIT_NOT,
  IR_OP_I64_BIT_OR,
  IR_OP_I64_BIT_XOR,
  IR_OP_I64_CLZ,
  IR_OP_I64_CONST,
  IR_OP_I64_CTZ,
  IR_OP_I64_IS_EQ,
  IR_OP_I64_IS_LE_S,
  IR_OP_I64_IS_LE_U,
  IR_OP_I64_IS_LT_S,
  IR_OP_I64_IS_LT_U,
  IR_OP_I64_MUL,
  IR_OP_I64_MUL_FULL_S,
  IR_OP_I64_MUL_FULL_U,
  IR_OP_I64_MUL_HI_S,
  IR_OP_I64_MUL_HI_U,
  IR_OP_I64_NEG,
  IR_OP_I64_REV,
  IR_OP_I64_ROL,
  IR_OP_I64_ROR,
  IR_OP_I64_SHL,
  IR_OP_I64_SHR_S,
  IR_OP_I64_SHR_U,
  IR_OP_I64_SUB,
  IR_OP_I64_TO_I32,
  IR_OP_I64_TO_I5,
  IR_OP_I64_TO_I6,
  IR_OP_COUNT,
} IR_Code;

typedef enum IR_TyCode : U16 {
  IR_TY_BOOL,
  IR_TY_F32,
  IR_TY_F64,
  IR_TY_FUNPTR,
  IR_TY_I32,
  IR_TY_I5,
  IR_TY_I6,
  IR_TY_I64,
  IR_TY_I8,
  IR_TY_V256,
  IR_TY_COUNT,
} IR_TyCode;
