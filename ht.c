#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "util.h"


typedef int ht_elem;

typedef struct {
  int full : 1;
  union {
    ht_elem k;
  };
} ht_sack;

typedef struct _ht_list {
  ht_elem k;
  struct _ht_list* next;
} ht_list;

typedef struct {
  size_t m;
  ht_list* t[];
} ht_slab;


ht_slab* ht_init(size_t m);
void ht_free(ht_slab* s);
void ht_insert(ht_slab* s, ht_elem k);
void ht_insert_all(ht_slab* s, const ht_elem* es, size_t len);
ht_sack ht_look(const ht_slab* s, ht_elem k);

size_t
ht_h(size_t m, ht_elem k)
{
  if (0 == k) {
    return 0;
  }
  else return ((k % m) ^ ht_h(m, k / m)) % m;
}

ht_slab*
ht_init(size_t m)
{
  ht_slab* ret = malloc(sizeof(ht_slab) + m * sizeof(ht_list));

  ret->m = m;
  memset(ret->t, 0, m * sizeof(ht_list*));
  return ret;
}

static void
_ht_list_free(ht_list* l)
{
  if (l != 0 ) {
    _ht_list_free(l->next);
  }
  free(l);
}

void
ht_free(ht_slab* s)
{
  size_t i;

  for (i = 0; i < s->m; i++) {
    _ht_list_free(s->t[i]);
  }
  free(s);
}

void
ht_insert(ht_slab* s, ht_elem k)
{
  size_t off = ht_h(s->m, k);
  ht_list* l = s->t[off];
  ht_list* c = malloc(sizeof(*c));

  c->next = l;
  c->k = k;
  s->t[off] = c;
}

void
ht_insert_all(ht_slab* s, const ht_elem* es, size_t len)
{
  if (len != 0) {
    ht_insert(s, *es);
    ht_insert_all(s, es + 1, len - 1);
  }
}

ht_sack
ht_look(const ht_slab* s, ht_elem k)
{
  const ht_list* l = s->t[ht_h(s->m, k)];
  ht_sack        ret;

  memset(&ret, 0, sizeof(ret));
  for (; l; l = l->next) {
    if (l->k == k) {
      ret.full = 1;
      ret.k = l->k;
      break;
    }
  }
  return ret;
}

static void
_ht_list_pretty(ht_list* l)
{
  printf("0x%02x", l->k);
  if (0 != l->next) {
    printf(" ");
    _ht_list_pretty(l->next);
  }
  else printf("\n");
}

static void
_ht_pretty(ht_slab* s)
{
  int i;

  for (i = 0; i < s->m; i++) {
    if (s->t[i]) {
      _ht_list_pretty(s->t[i]);
    }
  }
}

static const ht_elem es[] = {4, 5, 6, 7, 8, 7, 127, 10, 20, 30, 40, 51, 2, 3};

int
main(int argc, char* argv[])
{
  ht_slab* s;
  int i;

  s = ht_init(7);
  ht_insert_all(s, es, LEN(es));
  _ht_pretty(s);

  ht_free(s);
  return 0;
}
