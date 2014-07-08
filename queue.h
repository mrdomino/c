struct _list;
typedef struct _queue queue_t;

queue_t* queue_new();
void queue_free(queue_t* q);
#define ENQUEUE(q, x) _enqueue(q, &(x), sizeof(QUEUE_TYPE))
#define DEQUEUE(q) (*(QUEUE_TYPE*)_dequeue(q))
#define EMPTY(q) _empty(q)


void _enqueue(struct _queue* q, const void* d, size_t sz);
void* _dequeue(struct _queue* q);
int _empty(struct _queue* q);

