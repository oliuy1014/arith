#include <stdio.h>
#include <stdlib.h>
#include "compress40.h"
#include "image_vs_a2.h"
#include "cells_vs_blocks.h"
#include "rgb_vs_vcs.h"
#include "pnm.h"
#include "a2blocked.h"
#include "a2plain.h"
#include "blk_vs_cw.h"
#include "assert.h"

typedef struct Block_avg {
        unsigned a, pb_q, pr_q;
        int b, c, d;
} *Block_T;

void debug_print_blocked(A2Methods_UArray2 a2);

void compress40(FILE *input)
{
        A2Methods_UArray2 a2_i     = image_to_a2(input);
        A2Methods_UArray2 a2_vcs   = int_to_vcs(a2_i);
        A2Methods_UArray2 a2_blks  = cells_to_blocks(a2_vcs);
        A2Methods_UArray2 a2_cw    = blk_to_cw(a2_blks);
        assert(a2_cw != NULL);
        A2Methods_UArray2 a2_blks2 = cw_to_blk(a2_cw);
        A2Methods_UArray2 a2_vcs2  = blocks_to_cells(a2_blks2);
        A2Methods_UArray2 a2_i2    = vcs_to_int(a2_vcs2);
        a2_to_image(a2_i2);
}

void decompress40(FILE *input)
{
        (void) input;
        fprintf(stderr, "decompression not implemented\n");
        exit(0);
}