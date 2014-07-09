#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "util.h"


/*
 * Skip lists:
 *
 *   [ ]-------------------->[ ]
 *    v                       v
 *   [ ]---------[ ]-------->[ ]
 *    v           v           v
 *   [ ]---[ ]---[ ]-------->[ ]
 *    v     v     v           v
 *   [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *
 * For each node, we store a value and a pointer to the highest skip level.
 *
 * For each skip level, we store the next highest skip level, the node at this
 * level, and the next node at this level.
 */

typedef struct _sl {
} sl_t;

sl_node* sl_new()
{
}

void sl_insert(sl_node* head, int k)
{
}
