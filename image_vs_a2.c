#include"image_vs_a2.h"
#include "assert.h"
#include <stdlib.h>

#define DENOMINATOR 255

void copy_elem(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl);
A2Methods_UArray2 trim(A2Methods_UArray2 a2, int width, int height);
A2Methods_UArray2 image_to_a2(FILE *fp)
{
        assert(fp != NULL);
        A2Methods_T methods = uarray2_methods_blocked;
        Pnm_ppm image = Pnm_ppmread(fp, methods);
        int width = methods->width(image->pixels);
        int height = methods->height(image->pixels);
        int even_w = width - (width % 2);
        int even_h = height - (height % 2);
        A2Methods_UArray2 a2_i = trim(image->pixels, even_w, even_h);
        Pnm_ppmfree(&image);
        return a2_i;
}

void a2_to_image(A2Methods_UArray2 a2_i)
{
        Pnm_ppm final_image = malloc(sizeof(*final_image));
        A2Methods_T methods = uarray2_methods_blocked;
        final_image->pixels = (A2Methods_UArray2) a2_i;
        final_image->width = methods->width(a2_i);
        final_image->height = methods->height(a2_i);
        final_image->denominator = DENOMINATOR;
        final_image->methods = methods;
        Pnm_ppmwrite(stdout, final_image);
        Pnm_ppmfree(&final_image);
}

void copy_elem(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;
        A2Methods_T methods = uarray2_methods_blocked;
        A2Methods_UArray2 output_arr = *(A2Methods_UArray2 *) cl;
        Pnm_rgb rgb = (Pnm_rgb) elem;
        *(Pnm_rgb) methods->at(output_arr, col, row) = *rgb;
}

A2Methods_UArray2 trim(A2Methods_UArray2 a2, int width, int height)
{
        A2Methods_T methods = uarray2_methods_blocked;
        A2Methods_UArray2 a2_new = methods->new(width, height, 
                                                sizeof(struct Pnm_rgb));
        for (int c = 0; c < width; c++) {
                for (int r = 0; r < height; r++) {
                        Pnm_rgb elem = (Pnm_rgb) methods->at(a2, c, r);
                        *(Pnm_rgb) methods->at(a2_new, c, r) = *elem;
                }
        }
        return a2_new;
}