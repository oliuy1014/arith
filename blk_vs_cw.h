#include "bitpack.h"
#include "a2blocked.h"

typedef struct field_coords {
        int width, lsb;
} *fcoords;
A2Methods_UArray2 blk_to_cw(A2Methods_UArray2 a2_b);
A2Methods_UArray2 cw_to_blk(A2Methods_UArray2 a2_cw);
void SET_FORMAT(fcoords cw_coords[]);