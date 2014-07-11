#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pq.h"
#include "util.h"


struct _pq {
  size_t length;
  size_t heap_size;
  pq_elem* es;
};

#define LEFT(x) ((((x) + 1) << 1) - 1)
#define RIGHT(x) (((x) + 1) << 1)
#define PARENT(x) ((((x) + 1) >> 1) - 1)


static void
_min_heapify(pq_t* q, size_t p)
{
  size_t l, r, smallest;

  l = LEFT(p);
  r = RIGHT(p);
  if (p >= q->heap_size / 2) {
    return;
  }
  else {
    if (q->es[l].key < q->es[p].key) {
      smallest = l;
    }
    else smallest = p;
    if (r < q->heap_size && q->es[r].key < q->es[smallest].key) {
      smallest = r;
    }
    if (p != smallest) {
      SWAP(q->es[p], q->es[smallest]);
      _min_heapify(q, smallest);
    }
  }
}

static void
_build_min_heap(pq_t* q)
{
  size_t i;
  /*
   * All the leaf nodes are already heaps. We therefore only need to worry
   * about the floor(length/2) internal nodes.
   */
  size_t count = q->length / 2;

  q->heap_size = q->length;
  for (i = 0; i < count; i++) {
    _min_heapify(q, q->length / 2 - i - 1);
  }
}

pq_t*
pq_new(const pq_elem* es, size_t len)
{
  pq_t* ret = malloc(sizeof(*ret));

  ret->es = malloc(len * sizeof(*es));
  memcpy(ret->es, es, len * sizeof(*es));
  ret->length = len;
  ret->heap_size = 0;
  _build_min_heap(ret);
  return ret;
}

void
pq_free(pq_t* q)
{
  if (q) {
    free(q->es);
  }
  free(q);
}

static void
_decrease_key(pq_t* q, size_t x, int key)
{
  size_t p = PARENT(x);

  if (x == 0 || q->es[p].key < key) {
    q->es[x].key = key;
  }
  else {
    SWAP(q->es[x], q->es[p]);
    _decrease_key(q, p, key);
  }
}

void
pq_enqueue(pq_t* q, const pq_elem* e)
{
  int key = e->key;

  if (q->heap_size == q->length) {
    /* XX */
    q->length++;
    q->es = realloc(q->es, sizeof(pq_elem) * q->length);
  }
  q->es[q->heap_size].d = e->d;
  q->es[q->heap_size].key = INT_MAX;
  q->heap_size++;
  _decrease_key(q, q->heap_size - 1, key);
}

pq_elem
pq_extract_min(pq_t* q)
{
  q->heap_size--;
  SWAP(q->es[0], q->es[q->heap_size]);
  _min_heapify(q, 0);
  return q->es[q->heap_size];
}

int
pq_length(const pq_t* q)
{
  return q->heap_size;
}
