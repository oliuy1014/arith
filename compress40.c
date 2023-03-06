#include <stdio.h>
#include "compress40.h"
#include "image_vs_a2.h"
#include "int_vs_float.h"
#include "float_vs_vcs.h"
#include "cells_vs_blocks.h"
#include "pnm.h"
#include "a2blocked.h"
#include "assert.h"

typedef struct Block_avg {
        unsigned a, pb_q, pr_q;
        int b, c, d;
} *Block_T;

void debug_print_blocked(A2Methods_UArray2 a2);

void compress40(FILE *input)
{
        // A2Methods_T methods = uarray2_methods_blocked;
        A2Methods_UArray2 a2_i     = image_to_a2(input);
        A2Methods_UArray2 a2_f     = int_to_float(a2_i);
        A2Methods_UArray2 a2_vcs   = float_to_vcs(a2_f);
        A2Methods_UArray2 a2_blks  = cells_to_blocks(a2_vcs);
        A2Methods_UArray2 a2_vcs2  = blocks_to_cells(a2_blks);
        A2Methods_UArray2 a2_f2    = vcs_to_float(a2_vcs2);
        A2Methods_UArray2 a2_i2    = float_to_int(a2_f2);
        a2_to_image(a2_i2);
}

void decompress40(FILE *input)
{
        (void) input;
        fprintf(stderr, "decompression not implemented\n");
        exit(0);
}

void debug_print_blocked(A2Methods_UArray2 a2)
{
        A2Methods_T methods = uarray2_methods_blocked;
        int width = methods->width(a2);
        int height = methods->height(a2);

        for (int r = 0; r < height; r++) {
                for (int c = 0; c < width; c++) {
                        printf("(%d, %d) ", c, r);
                        Block_T elem = (Block_T) methods->at(a2, c, r);
                        printf("a: %d, b: %d, c: %d, d: %d, pb: %d, pr: %d ",
                        elem->a, elem->b, elem->c, elem->d, elem->pb_q, elem->pr_q);
                        printf("\n");
                }
        }
}