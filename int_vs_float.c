#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pnm.h"
#include "a2blocked.h"
#include "assert.h"

#define DENOMINATOR 255

typedef struct RGB_float {
        float red, blue, green;
} *RGB_float;

void i_to_f_apply(int col, int row, A2Methods_UArray2 a2, 
                      void *elem, void *cl);

void debug_print(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl);
                       
void f_to_i_apply(int col, int row, A2Methods_UArray2 a2,
                      void *elem, void *cl);

A2Methods_UArray2 int_to_float(A2Methods_UArray2 a2_i) 
{
        assert(a2_i != NULL);
        A2Methods_T methods = uarray2_methods_blocked;

        int width = methods->width(a2_i);
        int height = methods->height(a2_i);
        A2Methods_UArray2 a2_f = methods->new(width, height,
                                                    sizeof(struct RGB_float));
        methods->map_block_major(a2_i, i_to_f_apply, a2_f);
        methods->free(&a2_i);
        return a2_f;
}

void i_to_f_apply(int col, int row, A2Methods_UArray2 a2,
                      void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;

        /* read in array from cl */
        A2Methods_UArray2 a2_f = (A2Methods_UArray2) cl;
        assert(a2_f != NULL);
        float denom = (float) DENOMINATOR;
        A2Methods_T methods = uarray2_methods_blocked;

        /* initialize struct for rgb float data */
        Pnm_rgb rgb = (Pnm_rgb) elem;
        //fprintf(stderr, "elemb: %d", ((Pnm_rgb) elem)->blue);
        RGB_float float_data = malloc(sizeof(struct RGB_float));
        assert(float_data != NULL);
        
        /* convert rgb values to floats */
        float_data->green = (float)(rgb->green) / denom;
        float_data->blue = (float)(rgb->blue) / denom;
        float_data->red = (float)(rgb->red) / denom;

        *(RGB_float) methods->at(a2_f, col, row) = *float_data;
        free(float_data);
}

A2Methods_UArray2 float_to_int(A2Methods_UArray2 a2_f)
{
        assert(a2_f != NULL);
        const A2Methods_T methods = uarray2_methods_blocked;
        int width = methods->width(a2_f);
        int height = methods->height(a2_f);
        A2Methods_UArray2 a2_i = methods->new(width, height, sizeof(struct Pnm_rgb));
        methods->map_block_major(a2_f, f_to_i_apply, a2_i);
        methods->free(&a2_f);
        return a2_i;
}

void f_to_i_apply(int col, int row, A2Methods_UArray2 a2_f,
                      void *elem, void *cl)
{
        assert(a2_f != NULL);
        assert(elem != NULL);
        assert(cl != NULL);

        A2Methods_UArray2 a2_i = (A2Methods_UArray2) cl;
        assert(a2_i != NULL);

        A2Methods_T methods = uarray2_methods_blocked;

        RGB_float float_data = (RGB_float) elem;
        assert(float_data != NULL);
        Pnm_rgb int_data = malloc(sizeof(struct Pnm_rgb));
        assert(int_data != NULL);
        int_data->red   = (unsigned)roundf(float_data->red   * DENOMINATOR);
        int_data->green = (unsigned)roundf(float_data->green * DENOMINATOR);
        int_data->blue  = (unsigned)roundf(float_data->blue  * DENOMINATOR);
        *(Pnm_rgb) methods->at(a2_i, col, row) = *int_data;
        free(int_data);
}

void debug_print(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;

        A2Methods_UArray2 target_arr = (A2Methods_UArray2) cl;
        A2Methods_T methods = uarray2_methods_blocked;

        RGB_float test_data = (RGB_float) methods->at(target_arr, col, row);
        Pnm_rgb rgb = (Pnm_rgb) elem;
        fprintf(stderr, "ri: %d, rf: %f, gi: %d, gf: %f, bi: %d, bf: %f\n",
                rgb->red, test_data->red,
                rgb->green, test_data->green,
                rgb->blue, test_data->blue);
}

