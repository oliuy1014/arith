#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pnm.h"
#include "a2blocked.h"
#include "assert.h"

#define DENOMINATOR_C 255
#define DENOMINATOR_D 511

typedef struct RGB_float {
        float red, green, blue;
} *RGB_float;

void i_to_f_apply(int col, int row, A2Methods_UArray2 a2, 
                      void *elem, void *cl);

void debug_print(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl);
                       
void f_to_i_apply(int col, int row, A2Methods_UArray2 a2,
                      void *elem, void *cl);

int clamp_pos(int input, int pos_limit);

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
        float denom = (float) DENOMINATOR_C;
        A2Methods_T methods = uarray2_methods_blocked;

        /* initialize struct for rgb float data */
        Pnm_rgb int_data = (Pnm_rgb) elem;
        //fprintf(stderr, "elemb: %d", ((Pnm_rgb) elem)->blue);
        RGB_float float_data = malloc(sizeof(struct RGB_float));
        assert(float_data != NULL);
        
        /* convert rgb values to floats */
        float_data->green = (float)(int_data->green) / denom;
        float_data->blue = (float)(int_data->blue) / denom;
        float_data->red = (float)(int_data->red) / denom;
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
        int r   = (unsigned)roundf(float_data->red  * DENOMINATOR_D);
        int g = (unsigned)roundf(float_data->green * DENOMINATOR_D);
        int b  = (unsigned)roundf(float_data->blue  * DENOMINATOR_D);
        int_data->red = clamp_pos(r, DENOMINATOR_D);
        int_data->blue = clamp_pos(b, DENOMINATOR_D);
        int_data->green = clamp_pos(g, DENOMINATOR_D);
        *(Pnm_rgb) methods->at(a2_i, col, row) = *int_data;
        free(int_data);
}

int clamp_pos(int input, int pos_limit)
{
        input = (input <= pos_limit) ? input : pos_limit;
        input = (input >= 0) ? input : 0;
        return input;
}


/* TODO: remove printing template below: */
/* print float data */
        // for (int row = 0; row < height; row++) {
        //         for (int col = 0; col < width; col++) {
        //                 RGB_float elem = (RGB_float) methods->at(a2_f, col, row);
        //                 printf("r: %f, b: %f, g: %f\n", 
        //                 elem->red, elem->green, elem->blue);
        //         }
        // }
        /* print int rgbs : */
        // for (int row = 0; row < height; row++) {
        //         for (int col = 0; col < width; col++) {
        //                 Pnm_rgb elem = (Pnm_rgb) methods->at(a2_i, col, row);
        //                 printf("r: %d g: %d b: %d\n", 
        //                 elem->red, elem->green, elem->blue);
        //         }
        // }