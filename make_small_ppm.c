#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pnm.h"
#include "a2blocked.h"
#include "image_vs_a2.h"

A2Methods_UArray2 trim_smaller(A2Methods_UArray2 a2, int width, int height);
int main(int argc, char const *argv[])
{
        (void) argc;
        FILE *fp = fopen(argv[1], "rb");
        A2Methods_UArray2 old = image_to_a2(fp);
        A2Methods_T methods = uarray2_methods_blocked;
        A2Methods_UArray2 new = trim_smaller(old, methods->width(old), methods->height(old));
        a2_to_image(new);
        return 0;
}

A2Methods_UArray2 trim_smaller(A2Methods_UArray2 a2, int width, int height)
{
        A2Methods_T methods = uarray2_methods_blocked;
        A2Methods_UArray2 a2_new = methods->new(16, 16, 
                                                sizeof(struct Pnm_rgb));
        srand(time(NULL));   // Initialization, should only be called once.
        for (int c = 0; c < 16; c++) {
                for (int r = 0; r < 16; r++) {
                        int random = rand();
                        // fprintf(stderr, "looping again\n");
                        Pnm_rgb elem = (Pnm_rgb) methods->at(a2, random % width, random % height);
                        *(Pnm_rgb) methods->at(a2_new, c, r) = *elem;
                        // fprintf(stderr, "c = %d, cw = %d, r = %d, rw = %d\n", c, c2, r, r2);
                }
        }
        // fprintf(stderr, "returning a2new\n");
        return a2_new;
}
