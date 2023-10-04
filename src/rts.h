noreturn void RTS_panic(char const *);

inline void * RTS_alloc(size_t n) {
  void * p = malloc(n);
  if (! p) RTS_panic("RTS_alloc: memory allocation failed");
  return p;
}

inline void RTS_free(void * p) {
  free(p);
}
