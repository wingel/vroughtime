#include "algotest.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Edge{
    struct Edge *next_edge;
    struct Edge *prev_edge;

    int amount_edges;
    int chime;
    double value;
}Edge;

typedef struct EdgeList{
    Edge *root;
}EdgeList;

struct algoReqs{
    EdgeList *edge_list;

    int amount_responses;
    double lo;
    double hi;
    double adjustment;
    double uncertainty;
    int n;
    int bad;
    int wanted;
};

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

void sortedInsert(Edge** root, Edge* new_edge)
{
    Edge* current;
 
    // if list is empty
    if (*root == NULL)
        *root = new_edge;
 
    // if the node is to be inserted at the beginning
    // of the doubly linked list
    else if ((*root)->value >= new_edge->value) {
        new_edge->next_edge = *root;
        new_edge->next_edge->prev_edge = new_edge;
        *root = new_edge;
    }
 
    else {
        current = *root;
 
        // locate the node after which the new node
        // is to be inserted
        while (current->next_edge != NULL &&
               current->next_edge->value < new_edge->value){
            current = current->next_edge;
        }
 
        /*Make the appropriate links */
 
        new_edge->next_edge = current->next_edge;
 
        // if the new node is not inserted
        // at the end of the list
        if (current->next_edge != NULL)
            new_edge->next_edge->prev_edge = new_edge;
 
        current->next_edge = new_edge;
        new_edge->prev_edge = current;
    }
}

int insertEdge(EdgeList *edge_list, double value, int chime){
    Edge* new_edge = createEdge(value, chime);

    // Checks if memory is allocated correctly
    if(new_edge == NULL) return 1;

    sortedInsert(&(edge_list->root), new_edge);
}

void print_tree(algoReqs *algo)
{
    Edge *temp = algo->edge_list->root;
    while(temp->next_edge != NULL){
        printf("%f ", temp->value);
        temp = temp->next_edge;
    }
    printf("\n\n");
}

bool find_overlap(algoReqs *algo, double adjust, double uncert){

    algo->amount_responses++;

    if(insertEdge(algo->edge_list, adjust-uncert, -1) == 1){
        printf("Left edge fail\n");
        return false;
    }

    if(!insertEdge(algo->edge_list, adjust+uncert, +1) == 1){
        printf("Right edge fail\n");
        return false;
    }
    
    int max_allow = 0;
    max_allow = algo->amount_responses - algo->n;

    if (max_allow < 0){
        return false;
    }

    for(int i = 0; i<max_allow; i++){

        algo->wanted = algo->amount_responses - i;

        int chime = 0;
        double lo = 0;
        double hi = 0;

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
            current_edge = current_edge->next_edge;
        }

        chime = 0;
        // Reverse edge list
        while(current_edge->next_edge != NULL){
            current_edge = current_edge->next_edge;
        }

        // Find hi
        while(current_edge->prev_edge != NULL){
            chime += current_edge->chime;

            if (chime >= algo->wanted){
                hi = current_edge->value;
                break;
            }
            current_edge = current_edge->prev_edge;
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

double get_adjustment(algoReqs *algo)
{
    return algo->adjustment;
}

bool free_tree(algoReqs *algo)
{
    Edge *current;
    while(algo->edge_list->root != NULL){
        current = algo->edge_list->root;
        algo->edge_list->root = algo->edge_list->root->next_edge;
        free(current);
    }
    free(algo->edge_list);
    free(algo);
}