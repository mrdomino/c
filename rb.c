#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LEN(x) (sizeof (x) / sizeof *(x))


typedef enum {
  blk,
  red
} rb_color;

#define COLORCHAR(x) ((x) == blk ? '#' : '+')

typedef int rb_val;

typedef struct _rb_node {
  struct _rb_node* p;
  struct _rb_node* l;
  struct _rb_node* r;
  rb_color         c;
  rb_val           k;
} rb_node;

typedef struct {
  rb_node  nil;
  rb_node* root;
} rb_tree;


rb_tree* rb_tree_new(void);
void rb_tree_free(rb_tree* t);
rb_node* rb_node_new(rb_tree* t, rb_val k);
void rb_node_free(rb_node* n);

void rb_insert(rb_tree* t, rb_node* n);
void rb_insert_all(rb_tree* t, const rb_val* keys, size_t len);


int rotations, left_rotations, right_rotations;


static void
_rb_node_init(rb_node* n, rb_val k, rb_color c, rb_node* nil)
{
  n->l = n->r = n->p = nil;
  n->c = c;
  n->k = k;
}

rb_tree*
rb_tree_new()
{
  rb_tree* ret = malloc(sizeof *ret);

  _rb_node_init(&ret->nil, 0, blk, &ret->nil);
  ret->root = &ret->nil;
  return ret;
}

rb_node*
rb_node_new(rb_tree* t, rb_val k)
{
  rb_node* ret = malloc(sizeof *ret);

  _rb_node_init(ret, k, red, &t->nil);
  return ret;
}

void
rb_node_free(rb_node* n)
{
  free(n);
}

static void
_rb_node_free_all(rb_node* n, const rb_node* nil)
{
  if (n != nil) {
    _rb_node_free_all(n->l, nil);
    _rb_node_free_all(n->r, nil);
    rb_node_free(n);
  }
}

void
rb_tree_free(rb_tree* t)
{
  _rb_node_free_all(t->root, &t->nil);
  free(t);
}

static void
_print_node(const rb_node* n, const rb_node* nil)
{
  printf("%c%d", COLORCHAR(n->c), n->k);
  if (nil != n->l) {
    _print_node(n->l, nil);
  }
  if (nil != n->r) {
    _print_node(n->r, nil);
  }
  putchar(')');
}

static void
print_tree(const rb_tree* t)
{
  _print_node(t->root, &t->nil);
  puts("");
}

static void
_pretty_node(int depth, const rb_node* n, const rb_node* nil)
{
  int i;

  if (n->l != nil) {
    _pretty_node(depth + 1, n->l, nil);
  }
  for (i = 0; i < depth; i++) {
    printf("   ");
  }
  printf("%c%d\n", COLORCHAR(n->c), n->k);
  if (n->r != nil) {
    _pretty_node(depth + 1, n->r, nil);
  }
}

static void
pretty_tree(rb_tree* t)
{
  _pretty_node(0, t->root, &t->nil);
}

/*
 * Rotations:
 *
 *       |                 |
 *       y    <--left-<    x
 *      / \               / \
 *     x   c             a   y
 *    / \                   / \
 *   a   b    >-right->    b   c
 *
 */

static void
_rb_left_rotate(rb_tree* t, rb_node* x)
{
  rb_node* y = x->r;

  left_rotations++;
  rotations++;

  x->r = y->l;
  if (y->l != &t->nil) {
    y->l->p = x;
  }
  y->p = x->p;
  if (x->p == &t->nil) {
    t->root = y;
  }
  else if (x == x->p->l) {
    x->p->l = y;
  }
  else {
    assert(x == x->p->r);
    x->p->r = y;
  }
  y->l = x;
  x->p = y;
}

static void
_rb_right_rotate(rb_tree* t, rb_node* y)
{
  rb_node* x = y->l;

  right_rotations++;
  rotations++;

  y->l = x->r;
  if (x->r != &t->nil) {
    x->r->p = y;
  }
  x->p = y->p;
  if (y->p == &t->nil) {
    t->root = x;
  }
  else if (y == y->p->l) {
    y->p->l = x;
  }
  else {
    assert(y == y->p->r);
    y->p->r = x;
  }
  x->r = y;
  y->p = x;
}

static void
_rb_insert_fixup(rb_tree* t, rb_node* z)
{
  rb_node* y;

  while (z->p->c == red) {
    assert(t->nil.c == blk);
    if (z->p == z->p->p->l) {
      y = z->p->p->r;
      if (y->c == red) {
        printf("case 1l:\n");
        pretty_tree(t);

        z->p->c = blk;
        y->c = blk;
        z->p->p->c = red;
        z = z->p->p;
      }
      else {
        if (z == z->p->r) {
          printf("case 2l:\n");
          pretty_tree(t);

          z = z->p;
          _rb_left_rotate(t, z);
        }
        printf("case 3l:\n");
        pretty_tree(t);

        z->p->c = blk;
        z->p->p->c = red;
        _rb_right_rotate(t, z->p->p);
      }
    }
    else {
      assert(z->p == z->p->p->r);
      y = z->p->p->l;
      if (y->c == red) {
        printf("case 1r:\n");
        pretty_tree(t);

        z->p->c = blk;
        y->c = blk;
        z->p->p->c = red;
        z = z->p->p;
      }
      else {
        if (z == z->p->l) {
          printf("case 2r:\n");
          pretty_tree(t);

          z = z->p;
          _rb_right_rotate(t, z);
        }
        printf("case 3r:\n");
        pretty_tree(t);

        z->p->c = blk;
        z->p->p->c = red;
        _rb_left_rotate(t, z->p->p);
      }
    }
    printf("post:\n");
    pretty_tree(t);
    printf("\n");
  }
  t->root->c = blk;
}

void
rb_insert(rb_tree* t, rb_node* z)
{
  rb_node* y = &t->nil;
  rb_node* x = t->root;

  printf("inserting %d\n", z->k);
  while (x != &t->nil) {
    y = x;
    if (z->k < x->k) {
      x = x->l;
    }
    else x = x->r;
  }
  z->p = y;
  if (y == &t->nil) {
    t->root = z;
  }
  else if (z->k < y->k) {
    y->l = z;
  }
  else y->r = z;
  z->l = &t->nil;
  z->r = &t->nil;
  z->c = red;
  _rb_insert_fixup(t, z);
}

void
rb_insert_all(rb_tree* t, const rb_val* keys, size_t len)
{
  if (0 != len) {
    rb_node* z = rb_node_new(t, *keys);
    rb_insert(t, z);
    rb_insert_all(t, keys + 1, len - 1);
  }
}

static const int keys[] = {0, -1, -2, -3, -4, -5, -6, -7, 1, 2, 3, 4};

int
main(){
  rb_tree* t = rb_tree_new();
  rb_node* z;

  rb_insert_all(t, keys, LEN(keys));
  pretty_tree(t);
  rb_tree_free(t);
  printf("l:%d r:%d (%d)\n", left_rotations, right_rotations, rotations);
  return 0;
}
