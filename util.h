#define LEN(x) (sizeof (x) / sizeof *(x))
#define SWAP(x, y) _swap(&(x), &(y), sizeof(x))

void _swap(void* x, void* y, size_t sz);
