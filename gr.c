#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "util.h"

typedef struct _gr_list {
  size_t           v;
  int              w;
  struct _gr_list* n;
} gr_list;

typedef enum {
  gr_color_wht,
  gr_color_gry,
  gr_color_blk
} gr_color;

typedef struct {
  enum { typ_inf, typ_num } typ;
  int               v;
} inf_int_t;

typedef struct {
  inf_int_t d;
/* BFS/DFS: == */
  gr_color  c;
  size_t    p;
/* DFS: ====== */
  size_t    f;
/* =========== */
} gr_vert;

typedef struct {
  size_t n;
  gr_list** es;
  gr_vert* vs;
} gr_graph;

gr_graph*
gr_graph_new(size_t n)
{
  gr_graph* ret;
  size_t    i;

  ret = emalloc(sizeof(gr_graph));
  ret->es = ecalloc(n, sizeof(gr_list*));
  ret->vs = emalloc(n * sizeof(gr_vert));
  ret->n = n;
  for (i = 0; i < n; i++) {
    ret->vs[i].c = gr_color_wht;
    ret->vs[i].d.typ = typ_inf;
    ret->vs[i].d.v = 0;
    ret->vs[i].p = n;
  }
  return ret;
}

void
gr_graph_add_directed(gr_graph* g, size_t u, size_t v, int w)
{
  gr_list* un;
  assert(u < g->n);
  assert(v < g->n);

  un = emalloc(sizeof(*un));
  un->v = v;
  un->w = w;
  un->n = g->es[u];
  g->es[u] = un;
}

static void
_gr_list_free(gr_list* es)
{
  if (es) {
    _gr_list_free(es->n);
  }
  free(es);
}

void
gr_graph_free(gr_graph* g)
{
  size_t i;
  for (i = 0; i < g->n; i++) {
    _gr_list_free(g->es[i]);
  }
  free(g->es);
  free(g->vs);
  free(g);
}

static void
gr_bfs(gr_graph* g, size_t s)
{
  size_t*  u;
  size_t   i;
  gr_list* l;
  gr_vert* v;
  queue_t* q = queue_new();

  for (i = 0; i < g->n; i++) {
    if (i != s) {
      g->vs[i].c = gr_color_wht;
      g->vs[i].d.typ = typ_inf;
    }
    g->vs[i].p = g->n;
  }
  g->vs[s].c = gr_color_gry;
  g->vs[s].d.typ = typ_num;
  g->vs[s].d.v = 0;

  enqueue(q, &s, sizeof s);
  while (!empty(q)) {
    u = dequeue(q);
    for (l = g->es[*u]; l; l = l->n) {
      if (g->vs[l->v].c == gr_color_wht) {
        v = &g->vs[l->v];
        v->c = gr_color_gry;
        v->d.typ = typ_num;
        v->d.v = g->vs[*u].d.v + 1;
        v->p = *u;
        enqueue(q, &l->v, sizeof l->v);
      }
    }
    g->vs[*u].c = gr_color_blk;
    free(u);
  }
  queue_free(q);
}

static void
print_path(gr_graph* g, size_t s, size_t v)
{
  assert(s < g->n);
  assert(v < g->n);

  if (v == s) {
    printf("%lu", s);
  }
  else if (g->vs[v].p == g->n) {
    printf("no path from %lu to %lu exists", s, v);
  }
  else {
    print_path(g, s, g->vs[v].p);
    printf(" -> %lu", v);
  }
}

static size_t dfs_time;

static gr_list*
_gr_dfs_visit(gr_graph* g, size_t u, gr_list* acc)
{
  gr_list* l;
  gr_vert* v;
  gr_list* tmp = emalloc(sizeof(*tmp));

  dfs_time = dfs_time + 1;
  g->vs[u].d.typ = typ_num;
  g->vs[u].d.v = (int)dfs_time;  // XX
  g->vs[u].c = gr_color_gry;
  for (l = g->es[u]; l; l = l->n) {
    v = &g->vs[l->v];
    if (v->c == gr_color_wht) {
      v->p = u;
      acc = _gr_dfs_visit(g, l->v, acc);
    }
  }
  g->vs[u].c = gr_color_blk;
  dfs_time = dfs_time + 1;
  g->vs[u].f = dfs_time;
  tmp->v = u;
  tmp->n = acc;
  return tmp;
}

gr_list*
gr_dfs(gr_graph* g)
{
  size_t   i;
  gr_list* ret = 0;

  for (i = 0; i < g->n; i++) {
    g->vs[i].c = gr_color_wht;
    g->vs[i].p = g->n;
  }
  dfs_time = 0;
  for (i = 0; i < g->n; i++) {
    if (g->vs[i].c == gr_color_wht) {
      ret = _gr_dfs_visit(g, i, ret);
    }
  }
  puts("");
  return ret;
}

static void
_dump_edge(const gr_list* l)
{
  printf("%lu", l->v);
  if (l->n) {
    printf(", ");
    _dump_edge(l->n);
  }
}

void
gr_dump_graph(const gr_graph* g)
{
  size_t i;

  printf("{\n");
  for (i = 0; i < g->n; i++) {
    if (g->es[i]) {
      printf("  %lu, {", i);
      _dump_edge(g->es[i]);
      printf("}\n");
    }
  }
  printf("}\n");
}

typedef struct {
  size_t u;
  size_t v;
  int w;
} edge_t;

static void
_gr_add_all(gr_graph* g, const edge_t* es, size_t len)
{
  if (0 != len) {
    gr_graph_add_directed(g, es->u, es->v, es->w);
    _gr_add_all(g, es + 1, len - 1);
  }
}

static const edge_t edges[] = {
  {0,  1,  1},
  {1,  2,  1},
  {2,  3,  1},
  {0,  2,  1},
  {5,  6,  1},
  {7,  5,  1},
  {6,  8,  1},
  {8,  1,  1},
  {9,  1,  1},
  {10, 1,  1},
  {13, 10, 1},
  {13, 9,  1},
  {13, 8,  1},
  {3,  13, 1}
};

int
main()
{
  gr_graph* g = gr_graph_new(14);

  _gr_add_all(g, edges, LEN(edges));

  {
    gr_bfs(g, 0);
    print_path(g, 0, 1); puts("");
    print_path(g, 0, 2); puts("");
    print_path(g, 0, 3); puts("");
    print_path(g, 0, 13); puts("");
  }

  {
    gr_bfs(g, 13);
    print_path(g, 13, 0); puts("");
    print_path(g, 13, 3); puts("");
    print_path(g, 13, 7); puts("");
  }

  {
    gr_list* topsort = gr_dfs(g);

    puts("topological order:");
    _dump_edge(topsort);
    puts("");
    puts("");
    _gr_list_free(topsort);
  }

  gr_dump_graph(g);
  gr_graph_free(g);
  return 0;
}
