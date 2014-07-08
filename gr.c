#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

#define QUEUE_TYPE size_t
#include "queue.h"

typedef struct _gr_list {
  size_t           v;
  struct _gr_list* n;
} gr_list;

typedef enum {
  gr_color_wht,
  gr_color_gry,
  gr_color_blk
} gr_color;

typedef struct {
  size_t   d;
/* BFS/DFS: */
  gr_color c;
  size_t   p;
/* DFS: === */
  size_t   f;
/* ======== */
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
  
  ret = malloc(sizeof(gr_graph));
  ret->es = malloc(n * sizeof(gr_list*));
  ret->vs = malloc(n * sizeof(gr_vert));
  ret->n = n;
  memset(ret->es, 0, n * sizeof(gr_list*));
  for (i = 0; i < n; i++) {
    ret->vs[i].c = gr_color_wht;
    ret->vs[i].d = n;
    ret->vs[i].p = n;
  }
  return ret;
}

void
gr_graph_add_directed(gr_graph* g, size_t u, size_t v)
{
  gr_list* un;
  assert(u < g->n);
  assert(v < g->n);

  un = malloc(sizeof(*un));
  un->v = v;
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
  free(g);
}

static void
gr_bfs(gr_graph* g, size_t s)
{
  size_t   u;
  size_t   i;
  gr_list* l;
  gr_vert* v;
  queue_t* q = queue_new();

  for (i = 0; i < g->n; i++) {
    if (i != s) {
      g->vs[i].c = gr_color_wht;
      g->vs[i].d = g->n;
    }
    g->vs[i].p = g->n;
  }
  g->vs[s].c = gr_color_gry;
  g->vs[s].d = 0;

  ENQUEUE(q, s);
  while (!EMPTY(q)) {
    u = DEQUEUE(q);
    for (l = g->es[u]; l; l = l->n) {
      if (g->vs[l->v].c == gr_color_wht) {
        v = &g->vs[l->v];
        v->c = gr_color_gry;
        v->d = g->vs[u].d + 1;
        v->p = u;
        ENQUEUE(q, l->v);
      }
    }
    g->vs[u].c = gr_color_blk;
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

static void
_gr_dfs_visit(gr_graph* g, size_t u)
{
  gr_list* l;
  gr_vert* v;

  dfs_time = dfs_time + 1;
  g->vs[u].d = dfs_time;
  g->vs[u].c = gr_color_gry;
  for (l = g->es[u]; l; l = l->n) {
    v = &g->vs[l->v];
    if (v->c == gr_color_wht) {
      v->p = u;
      _gr_dfs_visit(g, l->v);
    }
  }
  g->vs[u].c = gr_color_blk;
  dfs_time = dfs_time + 1;
  g->vs[u].f = dfs_time;
}

void
gr_dfs(gr_graph* g)
{
  size_t i;

  for (i = 0; i < g->n; i++) {
    g->vs[i].c = gr_color_wht;
    g->vs[i].p = g->n;
  }
  dfs_time = 0;
  for (i = 0; i < g->n; i++) {
    if (g->vs[i].c == gr_color_wht) {
      _gr_dfs_visit(g, i);
    }
  }
  puts("");
}

typedef struct {
  size_t u;
  size_t v;
} edge_t;

static void
_gr_add_all(gr_graph* g, const edge_t* es, size_t len)
{
  if (0 != len) {
    gr_graph_add_directed(g, es->u, es->v);
    _gr_add_all(g, es + 1, len - 1);
  }
}

static const edge_t edges[] = {
  {0, 1},
  {1, 2},
  {2, 3},
  {0, 2},
  {5, 6},
  {7, 5},
  {6, 8},
  {8, 1},
  {9, 1},
  {10, 1},
  {13, 10},
  {13, 9},
  {13, 8},
  {3, 13}
};

int
main()
{
  gr_graph* g = gr_graph_new(50);

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

  gr_dfs(g);

  gr_graph_free(g);
  return 0;
}
