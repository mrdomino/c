#include <stdio.h>

#include "util.h"


/* p: idx of first element
 * q: idx of last element
 */
int
partition(int* a, int p, int r)
{
  int x = a[r];
  int i = p - 1;
  int j;

  for (j = p; j < r; j++) {
    if (a[j] <= x) {
      i++;
      SWAP(a[i], a[j]);
    }
  }
  SWAP(a[i + 1], a[r]);
  return i + 1;
}

/*
 * p: idx of first element to sort
 * r: idx of last element to sort
 */
void
quicksort(int* a, int p, int r)
{
  int q;

  if (p < r) {
    q = partition(a, p, r);
    quicksort(a, p, q - 1);
    quicksort(a, q + 1, r);
  }
}

static int as[] = {5, 3, 8, 12, 15, 28, 21, 37, 51, 12};

int
main() {
  int i;

  quicksort(as, 0, LEN(as) - 1);
  for (i = 0; i < LEN(as); i++) {
    printf("%d\n", as[i]);
  }
  return 0;
}
