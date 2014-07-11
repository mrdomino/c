typedef struct _pq pq_t;

typedef struct {
  int    key;
  void*  d;
} pq_elem;

pq_t* pq_new(const pq_elem* es, size_t sz);
void pq_free(pq_t* q);
void pq_enqueue(pq_t* q, const pq_elem* e);
pq_elem pq_extract_min(pq_t* q);
int pq_length(const pq_t* q);
