#include "prelude.h"
#include "rts.h"

__attribute__((noinline))
noreturn void RTS_panic(char * message) {
  fprintf(stderr, "%s", message);
  abort();
}

extern inline void * RTS_alloc(size_t);

extern inline void RTS_free(void *);
