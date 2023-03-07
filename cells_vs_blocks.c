#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "cells_vs_blocks.h"
#include "seq.h"
#include "arith40.h"


#define NUM_CELLS 4

typedef struct RGB_float {
        float red, blue, green;
} *RGB_float;

typedef struct Vcs {
        float y, pb, pr;
} *Vcs_T;

typedef struct Block_avg {
        unsigned a, pb_q, pr_q;
        int b, c, d;
} *Block_T;

typedef struct closure {
        A2Methods_UArray2 arr;
        Vcs_T cells[NUM_CELLS];
} *cl_T;
void c_to_b_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl);

void b_to_c_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl);

void debug_print_(A2Methods_UArray2 a2);

void quick_debug_print_(A2Methods_UArray2 a2);

float get_y1(Block_T block_data);
float get_y2(Block_T block_data);
float get_y3(Block_T block_data);
float get_y4(Block_T block_data);

int get_pb_q(Vcs_T arr[]);
int get_pr_q(Vcs_T arr[]);
int get_a(Vcs_T arr[]);
int get_b(Vcs_T arr[]);
int get_c(Vcs_T arr[]);
int get_d(Vcs_T arr[]);
float clamp(float input, float pos_limit);
float (*get_yp[4])(Block_T block_data);

A2Methods_UArray2 cells_to_blocks(A2Methods_UArray2 a2_vcs)
{
        assert(a2_vcs != NULL);
        const A2Methods_T methods = uarray2_methods_blocked;
        int width = methods->width(a2_vcs);
        int height = methods->height(a2_vcs);
        A2Methods_UArray2 a2_b = methods->new(width / 2, height / 2,
                                              sizeof(struct Block_avg));
        assert(a2_b != NULL);
        cl_T cl = malloc(sizeof(struct closure));
        assert(cl != NULL);
        cl->arr = a2_b;

        methods->map_block_major(a2_vcs, c_to_b_apply, cl);
        /* print vcs data */
        // for (int row = 0; row < height; row++) {
        //         for (int col = 0; col < width; col++) {
        //                 Vcs_T elem = (Vcs_T) methods->at(a2_vcs, col, row);
        //                 printf("y: %f, pb: %f, pr: %f\n", 
        //                 elem->y, elem->pb, elem->pr);
        //         }
        // }
        /* print block data */
        // for (int col = 0; col < width / 2; col++) {
        //         for (int row = 0; row < height / 2; row++) {
        //                 Block_T elem = (Block_T) methods->at(a2_b, col, row);
        //                 for (int i = 0; i < 4; i++) {
        //                         printf("a: %d, b: %d, c: %d, d: %d, pb: %d, pr: %d\n", 
        //                         elem->a, elem->b, elem->c, elem->d, elem->pb_q, elem->pr_q);
        //                 }
        //         }
        // }
        methods->free(&a2_vcs);
        free(cl);

        return a2_b;
}


// void quick_debug_print_(A2Methods_UArray2 a2)
// {
//         A2Methods_T methods = uarray2_methods_blocked;
//         int width = methods->width(a2);
//         int height = methods->height(a2);

//         for (int r = 0; r < height; r++) {
//                 for (int c = 0; c < width; c++) {
//                         Block_T elem = (Block_T) methods->at(a2, c, r);
//                         printf("%f ", elem->y);
//                 }
//                 printf("\n\n");
//         }
// }

void c_to_b_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl)
{
        assert(a2_vcs != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        A2Methods_UArray2 a2_b = (A2Methods_UArray2) cl;
        assert(a2_b != NULL);
        A2Methods_T methods = uarray2_methods_blocked;
        Vcs_T vcs_data = (Vcs_T) elem;
        assert(vcs_data != NULL);
        cl_T CL = (cl_T) cl;
        /* Switching from col major within blocks to row major*/
        int idx = 2 * (row % 2) + (col % 2);
        // fprintf(stderr, "index: %d, y: %f, pb: %f, pr: %f\n",
                // idx, vcs_data->y, vcs_data->pb, vcs_data->pr);
        
        
        // printf("y: %f, pb: %f, pr: %f\n", 
        // vcs_data->y, vcs_data->pb, vcs_data->pr);
       
        (CL->cells)[idx] = vcs_data;
        if ((col % 2 == 1) && (row % 2 == 1)) {
                Block_T block_data = malloc(sizeof(struct Block_avg));
                assert(block_data != NULL);
                block_data->a = get_a(CL->cells);
                block_data->b = get_b(CL->cells);
                block_data->c = get_c(CL->cells);
                block_data->d = get_d(CL->cells);
                block_data->pb_q = get_pb_q(CL->cells);
                block_data->pr_q = get_pr_q(CL->cells);
                *(Block_T) methods->at(CL->arr, (col + 1) / 2 - 1,
                                       (row + 1) / 2 - 1) = 
                                       *block_data;
                free(block_data);
        }
}

A2Methods_UArray2 blocks_to_cells(A2Methods_UArray2 a2_b) 
{
        assert(a2_b != NULL);
        const A2Methods_T methods = uarray2_methods_blocked;
        int width = methods->width(a2_b);
        int height = methods->height(a2_b);
        A2Methods_UArray2 a2_vcs = methods->new(width * 2, height * 2,
                                              sizeof(struct Vcs));
        assert(a2_vcs != NULL);
        methods->map_block_major(a2_b, b_to_c_apply, a2_vcs);
        /* print block data */
        // for (int row = 0; row < width; row++) {
        //         for (int col = 0; col < height; col++) {
        //                 Block_T elem = (Block_T) methods->at(a2_b, col, row);
        //                 for (int i = 0; i < 4; i++) {
        //                         printf("a: %d, b: %d, c: %d, d: %d, pb: %d, pr: %d\n", 
        //                         elem->a, elem->b, elem->c, elem->d, elem->pb_q, elem->pr_q);
        //                 }
        //         }
        // }
        /* print vcs data row-block-major */
        // for (int blk_r = 0; blk_r < height; blk_r += 2) {
        //         for (int blk_c = 0; blk_c < width; blk_c += 2) {
        //                 for (int i = 0; i < 2; i++) {
        //                         for (int j = 0; j < 2; j++) {
        //                                 Vcs_T elem = (Vcs_T) methods->at(a2_vcs, blk_c + j, blk_r + i);
        //                                 printf("y: %f, pb: %f, pr: %f\n", 
        //                                 elem->y, elem->pb, elem->pr);
        //                         }
                                
        //                 }
                        
        // }
        // }
        /* print vcs data row major */
        // for (int row = 0; row < height; row++) {
        //         for (int col = 0; col < width; col++) {
        //                 Vcs_T elem = (Vcs_T) methods->at(a2_vcs, col, row);
        //                 printf("y: %f, pb: %f, pr: %f\n", 
        //                 elem->y, elem->pb, elem->pr);
        //         }
        // }
        
        methods->free(&a2_b);
        return a2_vcs;
}

void b_to_c_apply(int col, int row, A2Methods_UArray2 a2_b, 
                  void *elem, void *cl)
{
        assert(a2_b != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        A2Methods_T methods = uarray2_methods_blocked;
        A2Methods_UArray2 a2_vcs = (A2Methods_UArray2) cl;
        assert(a2_vcs != NULL);

        Block_T block_data = (Block_T) elem;
        assert(block_data != NULL);
        
        Vcs_T vcs_1 = malloc(sizeof(struct Vcs));
        Vcs_T vcs_2 = malloc(sizeof(struct Vcs));
        Vcs_T vcs_3 = malloc(sizeof(struct Vcs));
        Vcs_T vcs_4 = malloc(sizeof(struct Vcs));
        assert(vcs_1 != NULL);
        assert(vcs_2 != NULL);
        assert(vcs_3 != NULL);
        assert(vcs_4 != NULL);

        vcs_1->y  = get_y1(block_data);
        vcs_2->y  = get_y2(block_data);
        vcs_3->y  = get_y3(block_data);
        vcs_4->y  = get_y4(block_data);

        vcs_1->pb = Arith40_chroma_of_index(block_data->pb_q);
        vcs_1->pr = Arith40_chroma_of_index(block_data->pr_q);
        vcs_2->pb = Arith40_chroma_of_index(block_data->pb_q);
        vcs_2->pr = Arith40_chroma_of_index(block_data->pr_q);
        vcs_3->pb = Arith40_chroma_of_index(block_data->pb_q);
        vcs_3->pr = Arith40_chroma_of_index(block_data->pr_q);
        vcs_4->pb = Arith40_chroma_of_index(block_data->pb_q);
        vcs_4->pr = Arith40_chroma_of_index(block_data->pr_q);
        Vcs_T vcs_arr[] = {vcs_1, vcs_2, vcs_3, vcs_4};
        for (int i = 0; i < NUM_CELLS; i++) {
                int c = col * 2 + i % 2;
                int r = row * 2 + i / 2;
                *(Vcs_T) methods->at(a2_vcs, c, r) = *vcs_arr[i];
                free(&(*vcs_arr[i]));
        }
}

float get_y1(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a/ 511) - (b / 50) -
                  (c / 50) + (d / 50);
        return y;
}

float get_y2(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / 511) - (b / 50) +
                  (c / 50) - (d / 50);
        return y;
}

float get_y3(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / 511) + (b / 50) -
                  ( c / 50) - (d / 50);
        return y;
}

float get_y4(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / 511) + (b / 50) +
                  (c / 50) + (d / 50);
        return y;
}

/***************************vcs to block funcs below***********************************/
int get_pb_q(Vcs_T arr[]) 
{
        float sum = 0;
        for (int i = 0; i < NUM_CELLS; i++) {
                sum += arr[i]->pb;
        }
        int pb_q = Arith40_index_of_chroma(sum / NUM_CELLS);
        return pb_q;
}

int get_pr_q(Vcs_T arr[]) 
{
        float sum = 0;
        for (int i = 0; i < NUM_CELLS; i++) {
                sum += arr[i]->pr;
        }
        int pr_q = Arith40_index_of_chroma(sum / NUM_CELLS);
        return pr_q;
}


int get_a(Vcs_T arr[])
{
        float sum = 0;
        for (int i = 0; i < NUM_CELLS; i++) {
                sum += arr[i]->y;
                printf("y%d: %f  ", i, arr[i]->y);
        }
        float a = sum / NUM_CELLS;
        int a_q = (int)roundf(511 * a);
        return a_q;
}

int get_b(Vcs_T arr[])
{
        float sum = 0;
        sum = -arr[0]->y - arr[1]->y + arr[2]->y + arr[3]->y;
        float b = clamp((sum / NUM_CELLS), 0.3);
        int b_q = (int)roundf(50 * b);
        return b_q;
}

int get_c(Vcs_T arr[])
{
        float sum = 0;
        sum = sum - arr[0]->y + arr[1]->y - arr[2]->y + arr[3]->y;
        float c = clamp((sum / NUM_CELLS), 0.3);
        int c_q = (int)roundf(50 * c);
        return c_q;
}

int get_d(Vcs_T arr[])
{
        float sum = 0;
        sum = sum + arr[0]->y - arr[1]->y - arr[2]->y + arr[3]->y;
        float d = clamp((sum / NUM_CELLS), 0.3);
        int d_q = (int)roundf(50 * d);
        return d_q;
}

float clamp(float input, float pos_limit)
{
        input = (input <= pos_limit) ? input : pos_limit;
        input = (input >= -pos_limit) ? input : -pos_limit;
        return input;
}

void debug_print_(A2Methods_UArray2 a2)
{
        A2Methods_T methods = uarray2_methods_blocked;
        int width = methods->width(a2);
        int height = methods->height(a2);

        for (int r = 0; r < height; r++) {
                for (int c = 0; c < width; c++) {
                        int blk_x = c / 2;
                        int blk_y = r / 2;
                        printf("(%d, %d) ", blk_x, blk_y);
                        Vcs_T elem = (Vcs_T) methods->at(a2, c, r);
                        printf("y: %f, pb: %f, pr: %f, ",
                        elem->y, elem->pb, elem->pr);
                        printf("\n");
                }
                printf("\n\n");
        }
}
