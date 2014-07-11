#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pq.h"
#include "util.h"


typedef struct _huff_node {
  size_t freq;
  char c;
  struct _huff_node* l;
  struct _huff_node* r;
} huff_node;

struct _huff_leaf {
  size_t f;
  char   c;
};

typedef struct {
  huff_node* r;
} hc_t;


hc_t*
hc_new(const struct _huff_leaf* ls, size_t len)
{
  size_t i;
  hc_t* ret = malloc(sizeof(*ret));
  pq_elem* es = malloc(sizeof(*es) * len);
  huff_node* cur;
  huff_node* x;
  huff_node* y;
  pq_elem cur_e;
  pq_t* q;

  assert(len > 1);
  for (i = 0; i < len; i++) {
    cur = malloc(sizeof(*cur));
    cur->freq = ls[i].f;
    cur->c = ls[i].c;
    cur->l = cur->r = 0;

    es[i].key = cur->freq;
    es[i].d = cur;
  }

  q = pq_new(es, len);
  for (i = 0; i < len - 1; i++) {
    assert(pq_length(q) >= 2);
    x = pq_extract_min(q).d;
    y = pq_extract_min(q).d;
    cur = malloc(sizeof(*cur));
    cur->freq = x->freq + y->freq;
    cur->c = 0;
    cur->l = x;
    cur->r = y;
    cur_e.key = cur->freq;
    cur_e.d = cur;
    pq_enqueue(q, &cur_e);
  }
  cur_e = pq_extract_min(q);
  ret->r = (huff_node*)cur_e.d;
  assert(pq_length(q) == 0);
  pq_free(q);
  free(es);
  return ret;
}

static void
_hc_node_free(huff_node* n)
{
  if (n) {
    _hc_node_free(n->l);
    _hc_node_free(n->r);
  }
  free(n);
}

void
hc_free(hc_t* h)
{
  if (h) {
    _hc_node_free(h->r);
  }
  free(h);
}

int
_node_bits_in_c(const huff_node* n, char c)
{
  int r;

  if (0 == n) {
    r = -1;
  }
  else if (0 == n->l && 0 == n->r) {
    if (c == n->c) {
      r = 0;
    }
    else r = -1;
  }
  else {
    r = _node_bits_in_c(n->l, c);
    if (r == -1) {
      r = _node_bits_in_c(n->r, c);
    }
    if (r != -1) {
      r++;
    }
  }
  return r;
}

int
_node_bits_in(const huff_node* n, const char* c)
{
  int r1, r2;
  if (*c == '\0') {
    return 0;
  }
  else {
    r1 = _node_bits_in_c(n, *c);
    if (-1 == r1) {
      return -1;
    }
    else {
      r2 = _node_bits_in(n, c + 1);
      if (-1 == r2) {
        return -1;
      }
      else return r1 + r2;
    }
  }
}

int
hc_bits_in(const hc_t* h, const char* c)
{
  return _node_bits_in(h->r, c);
}

static void
_node_print(int d, const huff_node* n) {
  int i;

  if (n->l) {
    _node_print(d + 1, n->l);
  }
  for (i = 0; i < d; i++) {
    printf("   ");
  }
  if (0 != n->c) {
    printf(".%c\n", n->c);
  }
  else printf(".\n");
  if (n->r) {
    _node_print(d + 1, n->r);
  }
}

void
hc_print(const hc_t* h)
{
  _node_print(0, h->r);
}


static const struct _huff_leaf ls[] = {
  { .f = 45, .c = 'a' },
  { .f = 23, .c = 'b' },
  { .f = 22, .c = 'c' },
  { .f = 25, .c = 'd' },
  { .f = 50, .c = 'e' },
  { .f = 20, .c = 'f' },
  { .f = 15, .c = 'g' },
  { .f = 12, .c = 'h' },
  { .f = 23, .c = 'i' },
  { .f = 34, .c = 'n' },
  { .f = 7,  .c = 'v' },
  { .f = 3,  .c = 'x' },
  { .f = 5,  .c = 'y' },
  { .f = 4,  .c = 'z' }
};

int
main(int argc, char* argv[])
{
  hc_t* h = hc_new(ls, LEN(ls));

  hc_print(h);
  printf("%d\n", hc_bits_in(h, "aeaeae"));
  printf("%d\n", hc_bits_in(h, "xyxyxy"));
  printf("%d\n", hc_bits_in(h, "a"));
  printf("%d\n", hc_bits_in(h, "e"));
  printf("%d\n", hc_bits_in(h, "n"));
  printf("%d\n", hc_bits_in(h, "v"));
  printf("%d\n", hc_bits_in(h, "x"));
  printf("%d\n", hc_bits_in(h, "y"));
  printf("%d\n", hc_bits_in(h, "abcdefghijklmnopqrstuvwxyz"));
  hc_free(h);
  return 0;
}
