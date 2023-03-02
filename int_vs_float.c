#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "a2blocked.h"
#include "assert.h"
#include "mem.h"

typedef struct RGB_float {
        float red, green, blue;
} *RGB_float; 

typedef struct array_and_denominator {
        A2Methods_UArray2 arr;
        float denom;
        A2Methods_T meths;
} arr_and_denom;

void convert_to_float(int col, int row, A2Methods_UArray2 a2, 
                      void *elem, void *cl);
void debug_print(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl);
A2Methods_UArray2 trim(A2Methods_UArray2 a2, int width, int height, 
                       A2Methods_T methods);

A2Methods_UArray2 int_to_float(Pnm_ppm image) 
{
        assert(image != NULL);
        A2Methods_T methods = uarray2_methods_blocked;
        A2Methods_UArray2 pixmap = image->pixels;

        /* TODO: implement trimming below */
        // Potential method of trimming before mapping:
        int even_w = image->width - (image->width % 2);
        int even_h = image->height - (image->height % 2);
        if (even_w != (int) image->width || even_h != (int) image->height) {
                pixmap = trim(pixmap, even_w, even_h, methods);
        }
        // After this, use trimmed width and height for rest of function
        
        arr_and_denom cl;
        // A2Methods_UArray2 output_arr = methods->new(image->width, image->height,
        //                                             sizeof(RGB_float));
        A2Methods_UArray2 output_arr = methods->new(even_w, even_h,
                                                    sizeof(RGB_float));
        cl.arr = output_arr;
        cl.denom = (float) image->denominator;
        cl.meths = methods;
        fprintf(stderr, "denominator %f\n", cl.denom);
        image->methods->map_block_major(pixmap, convert_to_float, &cl);
        image->methods->map_block_major(pixmap, debug_print, &cl);
        return cl.arr;
}

void convert_to_float(int col, int row, A2Methods_UArray2 a2,
                      void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;

        /* read in array and denominator from closure struct */
        arr_and_denom *closure = cl;
        A2Methods_UArray2 target_arr = closure->arr;
        float denom = closure->denom;
        A2Methods_T methods = closure->meths;

        /* initialize struct for rgb float data */
        Pnm_rgb rgb = (Pnm_rgb) elem;
        RGB_float float_data = malloc(sizeof(*float_data));
        
        /* convert rgb values to floats */
        float_data->red = (float)(rgb->red) / denom;
        float_data->green = (float)(rgb->green) / denom;
        float_data->blue = (float)(rgb->blue) / denom;

        /* Load data into new array */
        *(RGB_float*) methods->at(target_arr, col, row) = float_data;
        // RGB_float test_data = *(RGB_float*) methods->at(target_arr, col, row);
        // printf("ri: %d, rf: %f, gi: %d, gf: %f, bi: %d, bf: %f\n",
        //         rgb->red, test_data->red,
        //         rgb->green, test_data->green,
        //         rgb->blue, test_data->blue);
}

void debug_print(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;

        /* read in array and denominator from closure struct */
        arr_and_denom *closure = cl;
        A2Methods_UArray2 target_arr = closure->arr;
        A2Methods_T methods = closure->meths;

        /* initialize struct for rgb float data */
        RGB_float test_data = *(RGB_float*) methods->at(target_arr, col, row);
        Pnm_rgb rgb = (Pnm_rgb) elem;
        printf("ri: %d, rf: %f, gi: %d, gf: %f, bi: %d, bf: %f\n",
                rgb->red, test_data->red,
                rgb->green, test_data->green,
                rgb->blue, test_data->blue);
}

A2Methods_UArray2 trim(A2Methods_UArray2 a2, int width, int height, 
                       A2Methods_T methods)
{
        A2Methods_UArray2 a2_new = methods->new(width, height, 
                                                sizeof(Pnm_rgb));
        for (int c = 0; c < width; c++) {
                for (int r = 0; r < height; r++) {
                        *(Pnm_rgb*) methods->at(a2_new, c, r) = 
                        *(Pnm_rgb*) methods->at(a2, c, r);
                }
        }
        return a2_new;
}