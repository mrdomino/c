#define LEN(x) (sizeof (x) / sizeof *(x))
#define SWAP(x, y) _swap(&(x), &(y), sizeof(x))

void _swap(void* x, void* y, size_t sz);
void pretty_list(const int* as, size_t len);
