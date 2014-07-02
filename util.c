#include <stdio.h>
#include <string.h>
#include "util.h"

#ifdef __GNUC__
#define alloca __builtin_alloca
#endif

void _swap(void* x, void* y, size_t sz)
{
  void* tmp = alloca(sz);

  memcpy(tmp, x, sz);
  memmove(x, y, sz);
  memcpy(y, tmp, sz);
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
