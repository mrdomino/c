#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct _list_node {
  void*              d;
  struct _list_node* n;
} list_node_t;

/*
 * out       inn
 *  |         |
 *  v         v
 * +-+  +-+  +-+
 * | |->| |->| |
 * +-+  +-+  +-+
 */

typedef void (*ptr_func_t)(void*);

typedef struct {
  list_node_t* inn;
  list_node_t* out;
  ptr_func_t   free_f;
} queue_t;

queue_t*
queue_init(ptr_func_t free_f)
{
  queue_t* ret = malloc(sizeof(*ret));

  ret->inn = ret->out = 0;
  ret->free_f = free_f;
  return ret;
}

void
_list_node_free(list_node_t* n, ptr_func_t free_f)
{
  if (n) {
    free_f(n->d);
    _list_node_free(n->n, free_f);
  }
  free(n);
}

void
queue_free(queue_t* q)
{
  _list_node_free(q->out, q->free_f);
  free(q);
}

void
enqueue(queue_t* q, void* d)
{
  list_node_t* nuu = malloc(sizeof(*nuu));

  nuu->d = d;
  nuu->n = 0;
  if (0 == q->inn) {
    assert(0 == q->out);
    q->inn = q->out = nuu;
  }
  else {
    assert(0 == q->inn->n);
    q->inn->n = nuu;
    q->inn = nuu;
  }
}

void*
dequeue(queue_t* q)
{
  list_node_t* tmp = q->out;
  void*        ret = 0;

  if (0 != tmp) {
    if (tmp->n) {
      q->out = tmp->n;
    }
    else {
      assert(tmp == q->inn);
      q->inn = q->out = 0;
    }
    ret = tmp->d;
    free(tmp);
  }
  return ret;
}


typedef struct _tree_node {
  int d;
  struct _tree_node* l;
  struct _tree_node* r;
} tree_node_t;

typedef void (*visit_func_t)(tree_node_t*, void*);


int
dfs(tree_node_t* t, int x, int* out)
{
  if (0 != t) {
    printf("dfs:visit %d\n", t->d);
    if (t->d == x) {
      *out = t->d;
      return 0;
    }
    else {
      if (-1 == dfs(t->l, x, out)) {
        return dfs(t->r, x, out);
      }
      else return 0;
    }
  }
  else {
    return -1;
  }
}

int
_bfs_help(queue_t* q, int x, int* out)
{
  tree_node_t* t = dequeue(q);

  if (0 == t) {
    printf("bfs:done\n");
    queue_free(q);
    return -1;
  }
  else {
    printf("bfs:visit %d\n", t->d);
    if (x == t->d) {
      *out = t->d;
      queue_free(q);
      return 0;
    }
    else {
      if (t->l) {
        enqueue(q, t->l);
      }
      if (t->r) {
        enqueue(q, t->r);
      }
      return _bfs_help(q, x, out);
    }
  }
}

static void
noop(void* unused) {}

int
bfs(tree_node_t* t, int x, int* out)
{
  queue_t* q = queue_init(noop);

  enqueue(q, t);
  return _bfs_help(q, x, out);
}

tree_node_t* tree_new(int d)
{
  tree_node_t* ret = malloc(sizeof(*ret));

  ret->d = d;
  ret->l = ret->r = 0;
  return ret;
}

void
preorder(tree_node_t* t, visit_func_t visit_f, void* data)
{
  if (t) {
    visit_f(t, data);
    preorder(t->l, visit_f, data);
    preorder(t->r, visit_f, data);
  }
}

void
postorder(tree_node_t* t, visit_func_t visit_f, void* data)
{
  if (t) {
    postorder(t->l, visit_f, data);
    postorder(t->r, visit_f, data);
    visit_f(t, data);
  }
}

static void
_visit_free(tree_node_t* t, void* unused)
{
  free(t);
}

void
tree_free(tree_node_t* t)
{
  postorder(t, _visit_free, 0);
}

int
main()
{
  int no;

  tree_node_t* root = tree_new(5);

  root->l = tree_new(4);
  root->r = tree_new(6);

  root->l->l = tree_new(7);
  root->l->r = tree_new(11);
  root->r->l = tree_new(30);
  root->r->r = tree_new(40);
  root->l->r->l = tree_new(60);
  root->l->r->r = tree_new(62);

  bfs(root, 31, &no);
  dfs(root, 31, &no);

  tree_free(root);
  return 0;
}
