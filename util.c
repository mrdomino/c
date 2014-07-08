#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#ifdef __GNUC__
#undef alloca
#define alloca __builtin_alloca
#endif

__inline__ void
_swap(void* x, void* y, size_t sz)
{
  void* tmp = alloca(sz);

  memcpy(tmp, x, sz);
  memmove(x, y, sz);
  memcpy(y, tmp, sz);
}

__inline__ void*
ecalloc(size_t c, size_t s)
{
  void* ret = calloc(c, s);

  assert(ret);
  return ret;
}

__inline__ void*
emalloc(size_t sz)
{
  void* ret = malloc(sz);

  assert(ret);
  return ret;
}

static void
_pretty_list_go(const int* as, size_t len)
{
  if (len > 0) {
    printf("%d", *as);
    if (len > 1) {
      printf(", ");
    }
    _pretty_list_go(as + 1, len - 1);
  }
}

void
pretty_list(const int* as, size_t len)
{
  printf("{");
  _pretty_list_go(as, len);
  printf("}\n");
}
