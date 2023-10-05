noreturn void RT_panic(char const *);

inline void * RT_alloc(size_t n) {
  void * p = malloc(n);
  if (! p) RT_panic("RT_alloc: memory allocation failed");
  return p;
}

inline void RT_free(void * p) {
  free(p);
}
