#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "float_vs_vcs.h"

#define BLOCKSIZE 2
typedef struct Vcs {
        float y, pb, pr;
} *Vcs_T;

typedef struct RGB_float {
        float red, blue, green;
} *RGB_float;

void f_to_v_apply(int col, int row, A2Methods_UArray2 a2,
                      void *elem, void *cl);
void v_to_f_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl);
void debug_vcs(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl);

A2Methods_UArray2 float_to_vcs(A2Methods_UArray2 a2_f)
{
        assert(a2_f != NULL);
        A2Methods_T methods = uarray2_methods_blocked;

        int width = methods->width(a2_f);
        int height = methods->height(a2_f);

        /* trim width and height if not even */
        A2Methods_UArray2 a2_vcs = methods->new_with_blocksize(width, height,
                                                          sizeof(struct Vcs),
                                                                  BLOCKSIZE);
        methods->map_block_major(a2_f, f_to_v_apply, a2_vcs);
        methods->free(&a2_f);
        return a2_vcs;
}

void f_to_v_apply(int col, int row, A2Methods_UArray2 a2,
                      void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;

        /* read in array from cl */
        A2Methods_UArray2 a2_vcs = (A2Methods_UArray2) cl;
        assert(a2_vcs != NULL);
        A2Methods_T methods = uarray2_methods_blocked;

        /* initialize struct for rgb float data */
        RGB_float float_data = (RGB_float) elem;
        Vcs_T vcs_data = malloc(sizeof(struct Vcs));
        assert(vcs_data != NULL);
        
        /* convert float values to vcs */
        vcs_data->y  =  0.299     * float_data->red   +
                        0.587     * float_data->green +
                        0.114     * float_data->blue;

        vcs_data->pb = -0.168736  * float_data->red   -
                        0.331264  * float_data->green +
                        0.5       * float_data->blue;

        vcs_data->pr =  0.5       * float_data->red   -
                        0.418688  * float_data->green -
                        0.081312  * float_data->blue;
        *(Vcs_T) methods->at(a2_vcs, col, row) = *vcs_data;
        free(vcs_data);
}

void debug_vcs(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;

        A2Methods_UArray2 a2_vcs = (A2Methods_UArray2) cl;
        A2Methods_T methods = uarray2_methods_blocked;

        Vcs_T test_data = (Vcs_T) methods->at(a2_vcs, col, row);
        RGB_float rgb = (RGB_float) elem;
        fprintf(stderr, "r: %f, g: %f, b: %f, y: %f, pb: %f, pr: %f\n",
                rgb->red, rgb->green, rgb->blue, 
                test_data->y, test_data->pb, test_data->pr);
}

A2Methods_UArray2 vcs_to_float(A2Methods_UArray2 a2_vcs)
{
        assert(a2_vcs != NULL);
        const A2Methods_T methods = uarray2_methods_blocked;
        int width = methods->width(a2_vcs);
        int height = methods->height(a2_vcs);
        A2Methods_UArray2 a2_f = methods->new(width, height, sizeof(struct RGB_float));
        methods->map_block_major(a2_vcs, v_to_f_apply, a2_f);
        methods->free(&a2_vcs);
        return a2_f;
}

void v_to_f_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl)
{
        assert(a2_vcs != NULL);
        assert(elem != NULL);
        assert(cl != NULL);

        A2Methods_UArray2 a2_f = (A2Methods_UArray2) cl;
        assert(a2_f != NULL);

        A2Methods_T methods = uarray2_methods_blocked;

        Vcs_T vcs_data = (Vcs_T) elem;
        assert(vcs_data != NULL);
        RGB_float float_data = malloc(sizeof(struct RGB_float));
        assert(float_data != NULL);

        float y  = vcs_data->y;
        float pb = vcs_data->pb;
        float pr = vcs_data->pr;
        
        float_data->red   = 1.0 * y + 0.0      * pb + 1.402    * pr;
        float_data->green = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
        float_data->blue  = 1.0 * y + 1.772    * pb + 0.0      * pr;
        
        *(RGB_float) methods->at(a2_f, col, row) = *float_data;
        free(float_data);
}