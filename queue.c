#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "util.h"

typedef struct _list {
  void*         d;
  struct _list* n;
} list_t;

struct _queue {
  list_t*       i;
  list_t*       o;
};

queue_t*
queue_new()
{
  return ecalloc(1, sizeof(queue_t));
}

static void
_list_free(list_t* l)
{
  if (l) _list_free(l->n);
  free(l);
}

void
queue_free(queue_t* q)
{
  _list_free(q->o);
  free(q);
}

void
enqueue(queue_t* q, const void* d, size_t sz)
{
  list_t* n;

  n = emalloc(sizeof(*n));
  n->d = emalloc(sz);
  memcpy(n->d, d, sz);
  n->n = 0;
  if (0 == q->i) {
    q->i = q->o = n;
  }
  else {
    q->i->n = n;
    q->i = n;
  }
}

void*
dequeue(queue_t* q)
{
  list_t* n;
  void*   d;

  if (0 == q->o) {
    return 0;
  }
  else {
    n = q->o;
    d = n->d;
    q->o = q->o->n;
    if (0 == q->o) {
      q->i = 0;
    }
    free(n);
    return d;
  }
}

int
empty(queue_t* q)
{
  return 0 == q->o;
}
