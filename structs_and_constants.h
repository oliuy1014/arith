#include "a2blocked.h"
#include "a2plain.h"

typedef struct Vcs {
        float y, pb, pr;
} *Vcs_T;

typedef struct Block_avg {
        unsigned a, pb_q, pr_q;
        int b, c, d;
} *Block_T;

#ifndef CONSTANTS
#define METHODS_P uarray2_methods_plain
#define METHODS_B uarray2_methods_blocked
#define DENOMINATOR_C 255
#define DENOMINATOR_D 255
#define A_W 9
#define BCD_W 5
#define PBR_W 4
#endif