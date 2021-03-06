#define mergesort _mergesort_xxx
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef mergesort

#include "util.h"


static int calls;


static void
merge(const int* a, size_t a_len, const int* b, size_t b_len, int* c)
{
  if (a_len == 0) {
    memmove(c, b, b_len * sizeof(int));
  }
  else if (b_len == 0) {
    memmove(c, a, a_len * sizeof(int));
  }
  else {
    if (*a < *b) {
      *c = *a;
      merge(a + 1, a_len - 1, b, b_len, c + 1);
    }
    else {
      *c = *b;
      merge(a, a_len, b + 1, b_len - 1, c + 1);
    }
  }
}

void
mergesort(int *restrict a, int *restrict b, size_t len)
{
  calls++;
  if (len > 1) {
    mergesort(a, b, (len + 1) / 2);
    mergesort(a + (len + 1) / 2, b, len / 2);
    memcpy(b, a, sizeof(int) * (len + 1) / 2);
    merge(b, (len + 1) / 2, a + (len + 1) / 2, len / 2, a);
  }
}

static int as[] = {5,4,3,2,1,20,25,34,8,7,7,4,6};
static int bs[(LEN(as) + 1)/2];

int
main()
{
  mergesort(as, bs, LEN(as));
  pretty_list(as, LEN(as));
  printf("calls:%d\n", calls);
  return 0;
}
