#include "algotest.h"
#include <stdlib.h>

typedef enum{
    RIGHT,
    LEFT
}direction;

struct algoReqs{
    int amount_responses;

    EdgeList *edge_list;

    int n;
    int bad;
    int wanted;
};

typedef struct Edge{
    Edge *next_edge;
    Edge *prev_edge;
    Edge *last_edge;
    int amount_edges;

    int value;
}Edge;

typedef struct EdgeList{
    Edge *left_root;
    Edge *right_root;
}EdgeList;

Edge* createEdge(int value){
    Edge *edge = calloc(1, sizeof(Edge));
    if(!edge) return NULL;

    edge->amount_edges = 0;
    edge->value = value;

    edge->next_edge = NULL;
    edge->prev_edge = NULL;
    edge->last_edge = NULL;

    return edge;
}

EdgeList* createEdgeList(){
    EdgeList *edge_list = calloc(1, sizeof(EdgeList));
    if(!edge_list) return NULL;

    edge_list->left_root = NULL;
    edge_list->right_root = NULL;

    return edge_list;
}

algoReqs* createAlgoReqs(int n){
    algoReqs *t = calloc(1, sizeof(algoReqs));

    t->edge_list = createEdgeList();
    if(!t->edge_list) return NULL;

    t->n = n;
    t->bad = 0;
    t->wanted = 0;

    return t;
}

void sortedInsert(Edge *root, Edge* new_edge){
    /* Special case for the head end */
    if (root == NULL || root->value >= new_edge->value) {
        new_edge->next_edge = root;
        root = new_edge;
    }
    else {
        Edge* current = root;
        /* Locate the node before the point of insertion
         */
        while (current->next_edge != NULL
               && current->next_edge->value < new_edge->value) {
            current = current->next_edge;
        }
        new_edge->next_edge = current->next_edge;
        current->next_edge->prev_edge = new_edge;
        current->next_edge = new_edge;
        new_edge->prev_edge = current;
    }
}

bool insertEdge(EdgeList *edge_list, int value, direction dir){
    Edge* new_edge = createEdge(value);
    if(new_edge == NULL) return false;

    if(dir == RIGHT){
        insertionSort(&(edge_list->right_root), &new_edge);
    }

    if(dir == LEFT){
        insertionSort(&(edge_list->left_root), &new_edge);
    }
}

bool findOverlap(algoReqs *algo, int adjust, int uncert){

    algo->amount_responses++;

    insertEdge(algo->edge_list, adjust-uncert, LEFT);
    insertEdge(algo->edge_list, adjust+uncert, RIGHT);

    int max_allow = algo->amount_responses - algo->n;
    if (max_allow < 0){
        return false
    }

    for(int i = 0; i<max_allow; i++){

        algo->wanted = algo->amount_responses - i;

        int chime = 0;
        int lo = 0;

        /* Del kvar att översätta.
        for e in edges:
                chime -= e[1]
                if chime >= self.wanted:
                    lo = e[0]
                    break

            chime = 0
            hi = None
            for e in reversed(edges):
                chime += e[1]
                if chime >= self.wanted:
                    hi = e[0]
                    break

            if lo is not None and hi is not None and lo <= hi:
                break

        else:
            return False

        self.lo = lo
        self.hi = hi
        self.adjustment = (lo + hi) / 2
        self.uncertainty = (hi - lo) / 2*/
    }

}