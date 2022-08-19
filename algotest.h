#include "stdbool.h"

typedef unsigned char uint8_t;

typedef struct algoReqs algoReqs;

algoReqs* createAlgoReqs(int n);

bool find_overlap(algoReqs *algo, int adjust, int uncert);

bool is_overlap(algoReqs *algo, int server_lo, int server_hi);