#pragma once

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#ifdef NDEBUG
#define DEBUG_ASSERT(X) do { (void) sizeof(X); } while (0)
#else
#define DEBUG_ASSERT(X) assert(X)
#endif

#define STATIC_ASSERT(X) static_assert(X)

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
typedef ptrdiff_t Int;

STATIC_ASSERT(sizeof(F32) == 4);
STATIC_ASSERT(sizeof(F64) == 8);
STATIC_ASSERT(sizeof(S8) == 1);
STATIC_ASSERT(sizeof(S16) == 2);
STATIC_ASSERT(sizeof(S32) == 4);
STATIC_ASSERT(sizeof(S64) == 8);
STATIC_ASSERT(sizeof(S128) == 16);
STATIC_ASSERT(sizeof(U8) == 1);
STATIC_ASSERT(sizeof(U16) == 2);
STATIC_ASSERT(sizeof(U32) == 4);
STATIC_ASSERT(sizeof(U64) == 8);
STATIC_ASSERT(sizeof(U128) == 16);

static inline F32 PEEK_F32(char * p) {
  F32 x;
  memcpy(&x, p, sizeof(F32));
  return x;
}

static inline F64 PEEK_F64(char * p) {
  F64 x;
  memcpy(&x, p, sizeof(F64));
  return x;
}

static inline U16 PEEK_U16(char * p) {
  U16 x;
  memcpy(&x, p, sizeof(U16));
  return x;
}

static inline U32 PEEK_U32(char * p) {
  U32 x;
  memcpy(&x, p, sizeof(U32));
  return x;
}

static inline U64 PEEK_U64(char * p) {
  U64 x;
  memcpy(&x, p, sizeof(U64));
  return x;
}

static inline S16 PEEK_S16(char * p) {
  S16 x;
  memcpy(&x, p, sizeof(S16));
  return x;
}

static inline S32 PEEK_S32(char * p) {
  S32 x;
  memcpy(&x, p, sizeof(S32));
  return x;
}

static inline S64 PEEK_S64(char * p) {
  S64 x;
  memcpy(&x, p, sizeof(S64));
  return x;
}

// TODO:
//
// For `PEEK_L*`, do a byteswap on big endian platforms.

static inline U16 PEEK_L16(char * p) {
  U16 x;
  memcpy(&x, p, sizeof(U16));
  return x;
}

static inline U32 PEEK_L32(char * p) {
  U32 x;
  memcpy(&x, p, sizeof(U32));
  return x;
}

static inline U64 PEEK_L64(char * p) {
  U64 x;
  memcpy(&x, p, sizeof(U64));
  return x;
}

// TODO:
//
// For `POKE_L*`, do a byteswap on big endian platforms.

static inline void POKE_L16(char * p, U16 x) {
  memcpy(p, &x, sizeof(U16));
}

static inline void POKE_L32(char * p, U32 x) {
  memcpy(p, &x, sizeof(U32));
}

static inline void POKE_L64(char * p, U64 x) {
  memcpy(p, &x, sizeof(U64));
}

static inline U8 B0(U64 x) {
  return (U8) x;
}

static inline U8 B1(U64 x) {
  return (U8) (x >> 8);
}

static inline U8 B2(U64 x) {
  return (U8) (x >> 16);
}

static inline U8 B3(U64 x) {
  return (U8) (x >> 24);
}

static inline U16 H0(U64 x) {
  return (U16) x;
}

static inline U16 H1(U64 x) {
  return (U16) (x >> 16);
}

static inline U16 H2(U64 x) {
  return (U16) (x >> 32);
}

static inline U16 H3(U64 x) {
  return (U16) (x >> 48);
}

static inline U32 W0(U64 x) {
  return (U32) x;
}

static inline U32 W1(U64 x) {
  return (U32) (x >> 32);
}

static inline U32 BBBB(U8 a, U8 b, U8 c, U8 d) {
  return (
      a
    | (U32) b << 8
    | (U32) c << 16
    | (U32) d << 24
  );
}

static inline U64 HHHH(U16 a, U16 b, U16 c, U16 d) {
  return
    ( (U64) a
    | (U64) ((U64) b << 16)
    | (U64) ((U64) c << 32)
    | (U64) ((U64) d << 48)
    );
}

static inline U64 HHW_(U16 a, U16 b, U32 c) {
  return
    ( (U64) a
    | (U64) ((U64) b << 16)
    | (U64) ((U64) c << 32)
    );
}

static inline U64 H___(U16 a) {
  return HHHH(a, 0, 0, 0);
}

static inline U64 HH__(U16 a, U16 b) {
  return HHHH(a, b, 0, 0);
}

static inline U64 HHH_(U16 a, U16 b, U16 c) {
  return HHHH(a, b, c, 0);
}

static inline U64 H_W_(U16 a, U32 b) {
  return HHW_(a, 0, b);
}

static inline Int clz64(U64 x) {
  return x ? __builtin_clzll(x) : 64;
}

static inline Int ctz64(U64 x) {
  return x ? __builtin_ctzll(x) : 64;
}

static inline U64 rev64(U64 x) {
  return __builtin_bswap64(x);
}

static inline U64 rol64(U64 x, Int k) {
  return x << (k & 0x3f) | x >> (- (unsigned int) k & 0x3f);
}

static inline U64 ror64(U64 x, Int k) {
  return x >> (k & 0x3f) | x << (- (unsigned int) k & 0x3f);
}

static inline U64 shl64(U64 x, Int k) {
  return x << (k & 0x3f);
}

static inline S64 asr64(S64 x, Int k) {
  return x >> (k & 0x3f);
}

static inline U64 lsr64(U64 x, Int k) {
  return x >> (k & 0x3f);
}
