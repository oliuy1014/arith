#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pnm.h"
#include "a2plain.h"
#include "image_pixmap_convert.h"

A2Methods_UArray2 trim_smaller(A2Methods_UArray2 a2, int width, int height);
int main(int argc, char const *argv[])
{
        (void) argc;
        FILE *fp = fopen(argv[1], "rb");
        A2Methods_UArray2 old = image_to_pixmap(fp);
        A2Methods_T methods = uarray2_methods_plain;
        A2Methods_UArray2 new = trim_smaller(old, methods->width(old), methods->height(old));
        pixmap_to_image(new);
        return 0;
}

A2Methods_UArray2 trim_smaller(A2Methods_UArray2 a2, int width, int height)
{
        A2Methods_T methods = uarray2_methods_plain;
        A2Methods_UArray2 a2_new = methods->new(16, 16, 
                                                sizeof(struct Pnm_rgb));
        srand(time(NULL));   // Initialization, should only be called once.
        for (int c = 0; c < 16; c++) {
                for (int r = 0; r < 16; r++) {
                        int r1 = (100 * rand());
                        int c2 = r1 % width;
                        if (c2 < 0) {
                                c2 = -c2;
                        }
                        int r2 = r1 % height;
                        if (r2 < 0) {
                                r2 = -r2;
                        }
                        Pnm_rgb elem = (Pnm_rgb) methods->at(a2, c2, r2);
                        *(Pnm_rgb) methods->at(a2_new, c, r) = *elem;
                }
        }
        methods->free(&a2);
        return a2_new;
}
