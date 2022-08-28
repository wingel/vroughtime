#include "stdbool.h"

typedef unsigned char uint8_t;

typedef struct algoReqs algoReqs;

// Creates a struct to store info
algoReqs* createAlgoReqs(int n);

// Calculates the overlap
bool find_overlap(algoReqs *algo, double adjust, double uncert);

// Function to see if a respons is whitin the overlap boundaries
bool is_overlap(algoReqs *algo, int server_lo, int server_hi);

// Returns the adjustment
double get_adjustment(algoReqs *algo);

// Prints all the edges
void print_tree(algoReqs *algo);

// Free all the memory allocated by the linked list
bool free_tree(algoReqs *algo);