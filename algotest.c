#include "algotest.h"
#include <stdlib.h>

struct algoReqs{
    EdgeList *edge_list;

    int amount_responses;
    int lo;
    int hi;
    int adjustment;
    int uncertainty;
    int n;
    int bad;
    int wanted;
};

typedef struct Edge{
    Edge *next_edge;
    Edge *prev_edge;

    int amount_edges;
    int chime;
    int value;
}Edge;

typedef struct EdgeList{
    Edge *root;
}EdgeList;

Edge* createEdge(int value, int chime){
    Edge *edge = calloc(1, sizeof(Edge));
    if(!edge) return NULL;

    edge->amount_edges = 0;
    edge->value = value;
    edge->chime = chime;

    edge->next_edge = NULL;
    edge->prev_edge = NULL;

    return edge;
}

EdgeList* createEdgeList(){
    EdgeList *edge_list = calloc(1, sizeof(EdgeList));
    if(!edge_list) return NULL;

    edge_list->root = NULL;

    return edge_list;
}

algoReqs* createAlgoReqs(int n){
    algoReqs *t = calloc(1, sizeof(algoReqs));

    t->edge_list = createEdgeList();
    if(!t->edge_list) return NULL;

    t->n = n;
    t->bad = 0;
    t->wanted = 0;
    t->adjustment = 0;
    t->uncertainty = 0;
    t->hi = 0;
    t->lo = 0;

    return t;
}

void sortedInsert(Edge *root, Edge* new_edge){
    /* Special case for the head end */
    if (root == NULL || root->value >= new_edge->value) {
        new_edge->next_edge = root;
        new_edge->prev_edge = NULL;
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

bool insertEdge(EdgeList *edge_list, int value, int chime){
    Edge* new_edge = createEdge(value, chime);
    if(new_edge == NULL) return false;  

    insertionSort(&(edge_list->root), &new_edge);
}

bool findOverlap(algoReqs *algo, int adjust, int uncert){

    algo->amount_responses++;

    insertEdge(algo->edge_list, adjust-uncert, -1);
    insertEdge(algo->edge_list, adjust+uncert, +1);
    
    int max_allow = 0;
    max_allow = algo->amount_responses - algo->n;
    if (max_allow < 0){
        return false
    }

    for(int i = 0; i<max_allow; i++){

        algo->wanted = algo->amount_responses - i;

        int chime = 0;
        int lo = 0;
        int hi = 0;

        // Make a copy of the root
        Edge *current_edge = algo->edge_list->root;

        // Find lo
        while(current_edge->next_edge != NULL)
        {
            chime -= current_edge->chime;
            if(chime >= algo->wanted)
            {
                lo = current_edge->value;
                break;
            }
        }

        chime = 0;
        // Reverse edge list
        while(current_edge != NULL){
            current_edge = current_edge->next_edge;
        }

        // Find hi
        while(current_edge->prev_edge != NULL){
            chime += current_edge->chime;

            if (chime >= algo->wanted){
                hi = current_edge->value;
                break;
            }
        }

        // Check if lo and hi is not 0 (May want to find a better criteria)
        if(lo != 0 && hi != 0 && lo <= hi){
            break;
        }
        else{
            return false;
        }

        algo->lo = lo;
        algo->hi = hi;
        algo->adjustment = (lo + hi) / 2;
        algo->uncertainty = (hi - lo) / 2;
        return true;
    }
}

bool is_overlap(algoReqs *algo, int server_lo, int server_hi){
    if(server_lo > algo->hi || server_hi < algo->lo){
        return true;
    }
}