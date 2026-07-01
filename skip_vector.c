#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_LEVEL 4
#define DIMENSION 4

typedef struct SkipNode {
    int doc_id;
    float embedding[DIMENSION];
    struct SkipNode* forward[MAX_LEVEL + 1];
} SkipNode;

typedef struct {
    SkipNode* header;
    int level;
} SkipList;

SkipNode* create_node(int doc_id, float* embed, int level) {
    SkipNode* sn = (SkipNode*)malloc(sizeof(SkipNode));
    sn->doc_id = doc_id;
    memcpy(sn->embedding, embed, sizeof(float) * DIMENSION);
    for (int i = 0; i <= MAX_LEVEL; i++) sn->forward[i] = NULL;
    return sn;
}

#ifdef _WIN32
    __declspec(dllexport) SkipList* init_skip_index();
    __declspec(dllexport) void insert_vector(SkipList* list, int doc_id, float* embed);
    __declspec(dllexport) int query_nearest_vector(SkipList* list, float* query_embed);
    __declspec(dllexport) void free_skip_list(SkipList* list);
#endif

SkipList* init_skip_index() {
    SkipList* list = (SkipList*)malloc(sizeof(SkipList));
    float dummy_embed[DIMENSION] = {0.0f};
    list->header = create_node(-1, dummy_embed, MAX_LEVEL);
    list->level = 0;
    return list;
}

void insert_vector(SkipList* list, int doc_id, float* embed) {
    SkipNode* update[MAX_LEVEL + 1];
    SkipNode* current = list->header;

    for (int i = list->level; i >= 0; i--) {
        // Sort nodes by the magnitude of their first dimension coordinate element
        while (current->forward[i] != NULL && current->forward[i]->embedding[0] < embed[0]) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    int rlevel = rand() % (MAX_LEVEL + 1);
    if (rlevel > list->level) {
        for (int i = list->level + 1; i <= rlevel; i++) update[i] = list->header;
        list->level = rlevel;
    }

    SkipNode* sn = create_node(doc_id, embed, rlevel);
    for (int i = 0; i <= rlevel; i++) {
        sn->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = sn;
    }
}

int query_nearest_vector(SkipList* list, float* query_embed) {
    SkipNode* current = list->header;
    float best_distance = 1e9f;
    int best_doc_id = -1;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->embedding[0] <= query_embed[0]) {
            current = current->forward[i];
        }
        if (current != list->header) {
            float dist = 0;
            for (int d = 0; d < DIMENSION; d++) {
                dist += pow(current->embedding[d] - query_embed[d], 2);
            }
            if (dist < best_distance) {
                best_distance = dist;
                best_doc_id = current->doc_id;
            }
        }
    }
    return best_doc_id;
}

void free_skip_list(SkipList* list) {
    SkipNode* current = list->header;
    while (current != NULL) {
        SkipNode* next = current->forward[0];
        free(current);
        current = next;
    }
    free(list);
}
