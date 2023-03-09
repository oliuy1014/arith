#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "cells_vs_blocks.h"
#include "seq.h"
#include "arith40.h"
#include "structs_and_constants.h"


#define NUM_CELLS 4
#define BCD_LIM 0.3
// #define A_BITS pow(2, A_W) - 1
// #define BCD_BITS roundf((pow(2, (BCD_W - 1)) - 1) / BCD_LIM)
#define A_BITS 511
#define BCD_BITS 50

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
float clamp(float input, float max, float min);
float (*get_yp[4])(Block_T block_data);

A2Methods_UArray2 cells_to_blocks(A2Methods_UArray2 a2_vcs)
{
        assert(a2_vcs != NULL);
        // fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);
        int width = METHODS_B->width(a2_vcs);
        int height = METHODS_B->height(a2_vcs);
        A2Methods_UArray2 a2_b = METHODS_P->new(width / 2, height / 2,
                                              sizeof(struct Block_avg));
        assert(a2_b != NULL);
        cl_T cl = malloc(sizeof(struct closure));
        assert(cl != NULL);
        cl->arr = a2_b;
        METHODS_B->map_default(a2_vcs, c_to_b_apply, cl);
        METHODS_B->free(&a2_vcs);
        // fprintf(stderr, "A: %d, BCD: %d\n", A_BITS, BCD_BITS);
        free(cl);
        return a2_b;
}

void c_to_b_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl)
{
        assert(a2_vcs != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        A2Methods_UArray2 a2_b = (A2Methods_UArray2) cl;
        assert(a2_b != NULL);
        Vcs_T vcs_data = (Vcs_T) elem;
        assert(vcs_data != NULL);
        cl_T CL = (cl_T) cl;
        /* Switching from col major within blocks to row major*/
        int idx = 2 * (row % 2) + (col % 2);
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
                *(Block_T) METHODS_P->at(CL->arr, (col + 1) / 2 - 1,
                                       (row + 1) / 2 - 1) = 
                                       *block_data;
                free(block_data);
        }
}

A2Methods_UArray2 blocks_to_cells(A2Methods_UArray2 a2_b) 
{
        assert(a2_b != NULL);
        int width = METHODS_P->width(a2_b);
        int height = METHODS_P->height(a2_b);
        A2Methods_UArray2 a2_vcs = METHODS_P->new(width * 2, height * 2,
                                              sizeof(struct Vcs));
        assert(a2_vcs != NULL);
        METHODS_P->map_default(a2_b, b_to_c_apply, a2_vcs);
        METHODS_P->free(&a2_b);
        return a2_vcs;
}

void b_to_c_apply(int col, int row, A2Methods_UArray2 a2_b, 
                  void *elem, void *cl)
{
        assert(a2_b != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        A2Methods_UArray2 a2_vcs = (A2Methods_UArray2) cl;
        assert(a2_vcs != NULL);

        Block_T block_data = (Block_T) elem;
        assert((int) roundf(block_data != NULL));
        
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
                *(Vcs_T) METHODS_P->at(a2_vcs, c, r) = *vcs_arr[i];
                free(&(*vcs_arr[i]));
        }
}

float get_y1(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / A_BITS) - (b / BCD_BITS) -
                  (c / BCD_BITS) + (d / BCD_BITS);
                // fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);
        return clamp(y, 1.0, 0.0);
}

float get_y2(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / A_BITS) - (b / BCD_BITS) +
                  (c / BCD_BITS) - (d / BCD_BITS);
                        //   fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);
        return clamp(y, 1.0, 0.0);
}

float get_y3(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / A_BITS) + (b / BCD_BITS) -
                  (c / BCD_BITS) - (d / BCD_BITS);
                        //   fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);
        return clamp(y, 1.0, 0.0);
}

float get_y4(Block_T block_data)
{
        float a = (float) block_data->a;
        float b = (float) block_data->b;
        float c = (float) block_data->c;
        float d = (float) block_data->d;
        float y = (a / A_BITS) + (b / BCD_BITS) +
                  (c / BCD_BITS) + (d / BCD_BITS);
                        //   fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);
        return clamp(y, 1.0, 0.0);
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
                // printf("y%d: %f  ", i, arr[i]->y);
        }
        float a = sum / NUM_CELLS;
        int a_q = (int)roundf(A_BITS * a);
                // fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);
        return a_q;
}

int get_b(Vcs_T arr[])
{
        float sum = 0;
        sum = -arr[0]->y - arr[1]->y + arr[2]->y + arr[3]->y;
        float b = clamp((sum / NUM_CELLS), 0.3, -0.3);
        int b_q = (int)roundf(BCD_BITS * b);
                // fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);

        return b_q;
}

int get_c(Vcs_T arr[])
{
        float sum = 0;
        sum = sum - arr[0]->y + arr[1]->y - arr[2]->y + arr[3]->y;
        float c = clamp((sum / NUM_CELLS), 0.3, -0.3);
        int c_q = (int)roundf(BCD_BITS * c);
                // fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);

        return c_q;
}

int get_d(Vcs_T arr[])
{
        float sum = 0;
        sum = sum + arr[0]->y - arr[1]->y - arr[2]->y + arr[3]->y;
        float d = clamp((sum / NUM_CELLS), 0.3, -0.3);
        int d_q = (int)roundf(BCD_BITS * d);
                // fprintf(stderr, "A: %f, BCD: %f\n", A_BITS, BCD_BITS);

        return d_q;
}

float clamp(float input, float max, float min)
{
        input = (input <= max) ? input : max;
        input = (input >= min) ? input : min;
        return input;
}