typedef enum A64_Ins : U8 {
  A64_ADD_Xd_Xn_Xm,
  A64_ADD_Xd_Xn_Xm_LSL,
  A64_ADD_Xd_Xn_Xm_LSR,
  A64_ADD_Xd_Xn_Xm_ASR,
} A64_Ins;

typedef enum A64_Reg : U8 {
  X0,
} A64_Reg,
