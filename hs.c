#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

typedef int hs_val;

typedef struct {
  size_t length;
  size_t heap_size;
  hs_val a[1];
} hs_heap;

#define PARENT(i) ((i) >> 1)
#define LEFT(i) ((i) << 1)
#define RIGHT(i) (1 | (i) << 1)
#define SWAP(x, y) do { \
  (x) = (x) ^ (y);            \
  (y) = (x) ^ (y);            \
  (x) = (x) ^ (y);            \
} while (0)

hs_heap*
hs_heap_new(const hs_val* keys, size_t n)
{
  hs_heap* ret = malloc(sizeof(hs_heap) + n * sizeof(hs_val));

  ret->length = n;
  ret->heap_size = 0;
  memcpy(ret->a + 1, keys, n * sizeof(hs_val));
  return ret;
}

void
hs_heap_free(hs_heap* h)
{
  free(h);
}

void
hs_max_heapify(hs_heap* h, size_t i)
{
  size_t l = LEFT(i);
  size_t r = RIGHT(i);
  size_t largest;

  if (l <= h->heap_size && h->a[l] > h->a[i]) {
    largest = l;
  }
  else largest = i;
  if (r <= h->heap_size && h->a[r] > h->a[largest]) {
    largest = r;
  }
  if (largest != i) {
    SWAP(h->a[i], h->a[largest]);
    hs_max_heapify(h, largest);
  }
}

void
hs_build_max_heap(hs_heap* h)
{
  size_t i;

  h->heap_size = h->length;
  for (i = h->length >> 1; i >= 1; i--) {
    hs_max_heapify(h, i);
  }
}

void
hs_sort(hs_heap* h)
{
  size_t i;

  for (i = h->length; i >= 2; i--) {
    SWAP(h->a[1], h->a[i]);
    h->heap_size = h->heap_size - 1;
    hs_max_heapify(h, 1);
  }
}

static void
_pretty_node(int depth, const hs_heap* h, int i)
{
  int j;

  if (LEFT(i) <= h->length) {
    _pretty_node(depth + 1, h, LEFT(i));
  }
  if (i <= h->length) {
    for (j = 0; j < depth; j++) {
      printf("   ");
    }
    printf("%d\n", h->a[i]);
  }
  if (RIGHT(i) <= h->length) {
    _pretty_node(depth + 1, h, RIGHT(i));
  }
}

void
pretty_heap(const hs_heap* h)
{
  _pretty_node(0, h, 1);
}

static const hs_val keys[] = {2, 5, 6, 8, 20, 3, 50, 9, 128, 2, 55, 37, 31, 47, 43};

int
main(int argc, char* argv[])
{
  hs_heap* h = hs_heap_new(keys, LEN(keys));

  hs_build_max_heap(h);
  printf("heap:\n");
  pretty_heap(h);
  hs_sort(h);
  printf("sorted:\n");
  pretty_heap(h);
  hs_heap_free(h);
  return 0;
}
