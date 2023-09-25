#ifdef NDEBUG
#define ASSERT(X) do { (void) sizeof(X); } while (0)
#else
#define ASSERT(X) assert(X)
#endif

typedef bool Bool;
typedef float F32;
typedef double F64;
typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;
typedef __int128_t S128;
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef __uint128_t U128;

static_assert(sizeof(Bool) == 1);
static_assert(sizeof(F32) == 4);
static_assert(sizeof(F64) == 8);
static_assert(sizeof(S8) == 1);
static_assert(sizeof(S16) == 2);
static_assert(sizeof(S32) == 4);
static_assert(sizeof(S64) == 8);
static_assert(sizeof(S128) == 16);
static_assert(sizeof(U8) == 1);
static_assert(sizeof(U16) == 2);
static_assert(sizeof(U32) == 4);
static_assert(sizeof(U64) == 8);
static_assert(sizeof(U128) == 16);

static inline F32 PEEK_F32(unsigned char * p) {
  F32 x;
  memcpy(&x, p, sizeof(F32));
  return x;
}

static inline F64 PEEK_F64(unsigned char * p) {
  F64 x;
  memcpy(&x, p, sizeof(F64));
  return x;
}

static inline U16 PEEK_U16(unsigned char * p) {
  U16 x;
  memcpy(&x, p, sizeof(U16));
  return x;
}

static inline U32 PEEK_U32(unsigned char * p) {
  U32 x;
  memcpy(&x, p, sizeof(U32));
  return x;
}

static inline U64 PEEK_U64(unsigned char * p) {
  U64 x;
  memcpy(&x, p, sizeof(U64));
  return x;
}

static inline S16 PEEK_S16(unsigned char * p) {
  S16 x;
  memcpy(&x, p, sizeof(S16));
  return x;
}

static inline S32 PEEK_S32(unsigned char * p) {
  S32 x;
  memcpy(&x, p, sizeof(S32));
  return x;
}

static inline S64 PEEK_S64(unsigned char * p) {
  S64 x;
  memcpy(&x, p, sizeof(S64));
  return x;
}

// TODO:
//
// For `PEEK_L*`, do a byteswap on big endian platforms.

static inline U16 PEEK_L16(unsigned char * p) {
  U16 x;
  memcpy(&x, p, sizeof(U16));
  return x;
}

static inline U32 PEEK_L32(unsigned char * p) {
  U32 x;
  memcpy(&x, p, sizeof(U32));
  return x;
}

static inline U64 PEEK_L64(unsigned char * p) {
  U64 x;
  memcpy(&x, p, sizeof(U64));
  return x;
}

static inline int clz64(U64 x) {
  return x ? __builtin_clzll(x) : 64;
}

static inline int ctz64(U64 x) {
  return x ? __builtin_ctzll(x) : 64;
}

static inline U64 rev64(U64 x) {
  return __builtin_bswap64(x);
}

static inline U64 rol64(U64 x, int k) {
  return x << (k & 0x3f) | x >> (- (unsigned int) k & 0x3f);
}

static inline U64 ror64(U64 x, int k) {
  return x >> (k & 0x3f) | x << (- (unsigned int) k & 0x3f);
}

static inline U64 shl64(U64 x, int k) {
  return x << (k & 0x3f);
}

static inline S64 asr64(S64 x, int k) {
  return x >> (k & 0x3f);
}

static inline U64 lsr64(U64 x, int k) {
  return x >> (k & 0x3f);
}
