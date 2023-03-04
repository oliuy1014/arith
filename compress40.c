#include <stdio.h>
#include "compress40.h"
#include "image_vs_a2.h"
#include "int_vs_float.h"
#include "float_vs_vcs.h"
#include "pnm.h"
#include "a2blocked.h"
#include "assert.h"
typedef struct RGB_float {
        float red, green, blue;
} *RGB_float;

void compress40(FILE *input)
{
        A2Methods_UArray2 a2_i     = image_to_a2(input);
        A2Methods_UArray2 a2_f     = int_to_float(a2_i);
        A2Methods_UArray2 a2_vcs   = float_to_vcs(a2_f);
        A2Methods_UArray2 a2_f2    = vcs_to_float(a2_vcs);
        A2Methods_UArray2 a2_i2    = float_to_int(a2_f2);
        a2_to_image(a2_i2);
}

void decompress40(FILE *input)
{
        (void) input;
        fprintf(stderr, "decompression not implemented\n");
        exit(0);
}

/*
closure struct:
{
        Vcs_T[] = block;
        Output array;
        counter;
}
*/