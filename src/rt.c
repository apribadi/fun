#include "prelude.h"
#include "rt.h"

__attribute__((noinline))
noreturn void RT_panic(char const * message) {
  fprintf(stderr, "panic: %s", message);
  abort();
}

extern inline void * RT_alloc(size_t);

extern inline void RT_free(void *);
