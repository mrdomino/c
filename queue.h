struct _list;
typedef struct _queue queue_t;

queue_t* queue_new();
void queue_free(queue_t* q);

void enqueue(queue_t* q, const void* d, size_t sz);
void* dequeue(queue_t* q);
int empty(struct _queue* q);

