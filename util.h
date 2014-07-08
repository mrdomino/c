#define LEN(x) (sizeof (x) / sizeof *(x))
#define SWAP(x, y) _swap(&(x), &(y), sizeof(x))

void* ecalloc(size_t c, size_t s);
void* emalloc(size_t sz);
void  pretty_list(const int* as, size_t len);
void  _swap(void* x, void* y, size_t sz);
