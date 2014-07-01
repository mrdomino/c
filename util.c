#include <alloca.h>
#include <string.h>
#include "util.h"

void _swap(void* x, void* y, size_t sz)
{
  void* tmp = alloca(sz);

  memcpy(tmp, x, sz);
  memcpy(x, y, sz);
  memcpy(y, tmp, sz);
}
