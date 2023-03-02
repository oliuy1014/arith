#include <stdio.h>
#include "compress40.h"
#include "int_vs_float.h"
#include "pnm.h"
#include "a2blocked.h"
#include "assert.h"

void compress40(FILE *input)
{
        assert(input != NULL);
        A2Methods_T methods = uarray2_methods_blocked;
        Pnm_ppm image = Pnm_ppmread(input, methods);
        A2Methods_UArray2 image_array = int_to_float(image);
        
        (void) image_array;
}

void decompress40(FILE *input)
{
        (void) input;
        fprintf(stderr, "decompression not implemented\n");
        exit(0);
}