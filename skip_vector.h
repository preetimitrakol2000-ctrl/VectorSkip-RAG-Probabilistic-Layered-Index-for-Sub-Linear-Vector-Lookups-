#ifndef SKIP_VECTOR_H
#define SKIP_VECTOR_H

typedef struct SkipNode SkipNode;
typedef struct SkipList SkipList;
SkipList* init_skip_index();
void insert_vector(SkipList* list, int doc_id, float* embed);
int query_nearest_vector(SkipList* list, float* query_embed);
void free_skip_list(SkipList* list);

#endif
