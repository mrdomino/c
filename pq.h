typedef struct _pq pq_t;

typedef struct {
  int    key;
  void*  d;
  size_t sz;
} pq_elem;

pq_t* pq_new();
void pq_free(pq_t* q);
void pq_insert(pq_t* q, const pq_elem* e);
void pq_insert_all(pq_t* q, const pq_elem* es, size_t sz);
pq_elem pq_extract_min(pq_t* q);
