#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "pnm.h"
#include "rgb_vs_vcs.h"
#include "structs_and_constants.h"

#define BLOCKSIZE 2

void i_to_v_apply(int col, int row, A2Methods_UArray2 a2,
                      void *elem, void *cl);
void v_to_i_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl);
int clamp_to_int(float input, float max, float min);

float clamp_to_float(float input, float max, float min);

A2Methods_UArray2 int_to_vcs(A2Methods_UArray2 a2_i)
{
        assert(a2_i != NULL);
        int width = METHODS_P->width(a2_i);
        int height = METHODS_P->height(a2_i);
        A2Methods_UArray2 a2_vcs = METHODS_B->new_with_blocksize(width, height, 
                                                          sizeof(struct Vcs),
                                                                  BLOCKSIZE);
        METHODS_P->map_default(a2_i, i_to_v_apply, a2_vcs);
        METHODS_P->free(&a2_i);
        return a2_vcs;
}

void i_to_v_apply(int col, int row, A2Methods_UArray2 a2,
                      void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;

        /* read in array from cl */
        A2Methods_UArray2 a2_vcs = (A2Methods_UArray2) cl;
        assert(a2_vcs != NULL);
        /* initialize struct for rgb int data */
        Pnm_rgb int_data = (Pnm_rgb) elem;
        Vcs_T vcs_data = malloc(sizeof(struct Vcs));
        assert(vcs_data != NULL);
        float denom = DENOMINATOR_C;

        float r = int_data->red / denom;
        float g = int_data->green / denom;
        float b = int_data->blue / denom;
        
        /* convert float values to vcs */
        vcs_data->y  =  clamp_to_float(
                        (0.299    * r +
                        0.587     * g +
                        0.114     * b), 1, 0);

        vcs_data->pb = clamp_to_float
                        ((-0.168736 * r -
                        0.331264   * g +
                        0.5        * b), 0.5, -0.5);

        vcs_data->pr =  clamp_to_float
                        ((0.5     * r -
                        0.418688  * g -
                        0.081312  * b), 0.5, -0.5);
        *(Vcs_T) METHODS_B->at(a2_vcs, col, row) = *vcs_data;
        free(vcs_data);
}

A2Methods_UArray2 vcs_to_int(A2Methods_UArray2 a2_vcs)
{
        assert(a2_vcs != NULL);
        int width = METHODS_P->width(a2_vcs);
        int height = METHODS_P->height(a2_vcs);
        A2Methods_UArray2 a2_i = METHODS_P->new(width, height, sizeof(struct Pnm_rgb));
        METHODS_P->map_default(a2_vcs, v_to_i_apply, a2_i);
        METHODS_P->free(&a2_vcs);
        return a2_i;
}

void v_to_i_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl)
{
        assert(a2_vcs != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        A2Methods_UArray2 a2_i = (A2Methods_UArray2) cl;
        assert(a2_i != NULL);
        Vcs_T vcs_data = (Vcs_T) elem;
        assert(vcs_data != NULL);
        Pnm_rgb int_data = malloc(sizeof(struct Pnm_rgb));
        assert(int_data != NULL);
        float d = DENOMINATOR_D;
        float y  = vcs_data->y;
        float pb = vcs_data->pb;
        float pr = vcs_data->pr;
        int_data->red   = clamp_to_int((1.0 * y + 0.0      * pb + 1.402    * pr) * d, d, 0);
        int_data->green = clamp_to_int((1.0 * y - 0.344136 * pb - 0.714136 * pr) * d, d, 0);
        int_data->blue  = clamp_to_int((1.0 * y + 1.772    * pb + 0.0      * pr) * d, d, 0);
        *(Pnm_rgb) METHODS_P->at(a2_i, col, row) = *int_data;
        free(int_data);
}

int clamp_to_int(float input, float max, float min)
{
        input = (input <= max) ? input : max;
        input = (input >= min) ? input : min;
        return (int) input;
}

float clamp_to_float(float input, float max, float min)
{
        input = (input <= max) ? input : max;
        input = (input >= min) ? input : min;
        return input;
}