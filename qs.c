#include <stdio.h>

#include "util.h"


int swaps;
int calls;


/* p: idx of first element
 * q: idx of last element
 */
int
partition(int* a, int p, int r)
{
  int x = a[r];
  int i = p - 1;
  int j;

  calls++;

  for (j = p; j < r; j++) {
    if (a[j] <= x) {
      i++;
      SWAP(a[i], a[j]); swaps++;
    }
  }
  SWAP(a[i + 1], a[r]); swaps++;
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

static int as[] = {1, 5, 6, 8, 4, 3, 7, 12, 2, 9, 11, 10};

int
main() {
  quicksort(as, 0, LEN(as) - 1);
  pretty_list(as, LEN(as));
  printf("swaps:%d calls:%d\n", swaps, calls);
  return 0;
}
