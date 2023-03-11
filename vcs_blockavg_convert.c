/*****************************************************************************
 *
 *                           vcs_blockavg_convert.c
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary: 
 *      - Compression: Averages VCS data of 2x2 blocks of a UArray2 and 
 *                     converts this data to block average data, and loads to
 *                     a new UArray2 with half the width and height. 
 *      - Decompression: Converts UArray2 of block average data to UArray2 of
 *                       VCS data, distributing each block data struct over a
 *                       2x2 block of VCS data.
 *      
 *      
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "vcs_blockavg_convert.h"
#include "seq.h"
#include "arith40.h"
#include "blockavg_operations.h"

/**********struct definitions************************************************/

/* closure struct
 *
 * Purpose: Stores a UArray2 and array of Vcs data to be used by an apply
 *          function.
 * Members:
 *      - A2Methods_UArray2 arr: UArray2 to be populated by an apply
 *      - Vcs_T cells[NUM_CELLS]: array of Vcs_T to be aggregated into a
 *        block by the apply function.
 */
typedef struct closure {
        A2Methods_UArray2 arr;
        Vcs_T cells[NUM_CELLS];
} *cl_T;

void vcs_to_blockavg_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl);

void blockavg_to_vcs_apply(int col, int row, A2Methods_UArray2 a2_blk, 
                  void *elem, void *cl);

/**********vcs_to_blockavg****************************************************
 *
 * Purpose:
 *      Converts a UArray2 with VCS data to a UArray2 with block data.
 * Parameters: 
 *      A2Methods_UArray2 a2_vcs: Input array with VCS data.
 * Return: 
 *      UArray2 with block average data that is half the width and height of
 *      input array.
 * Expects:
 *      a2_vcs to not be NULL
 * Notes:
 *      Raises CRE if a2_vcs is NULL
 ****************************************************************************/
A2Methods_UArray2 vcs_to_blockavg(A2Methods_UArray2 a2_vcs)
{
        assert(a2_vcs != NULL);
        int width = METHODS_B->width(a2_vcs);
        int height = METHODS_B->height(a2_vcs);
        A2Methods_UArray2 a2_blk = METHODS_P->new(width / 2, height / 2,
                                              sizeof(struct Block_avg));
        assert(a2_blk != NULL);
        cl_T cl = malloc(sizeof(struct closure));
        assert(cl != NULL);
        cl->arr = a2_blk;
        METHODS_B->map_default(a2_vcs, vcs_to_blockavg_apply, cl);
        METHODS_B->free(&a2_vcs);
        free(cl);
        return a2_blk;
}

/*********vcs_to_blockavg_apply***********************************************
 *
 * Purpose:
 *      Apply function called by vcs_to_blockavg. Converts VCS data to 
 *      block data by aggregating four VCS structs in a 2x2 block and 
 *      computing block data, then loading onto UArray2 of block data
 *      expected to be inside *cl.
 * Parameters:
 *      int col: column at VCS data is in a2_vcs
 *      int row: row at VCS data is in a2_vcs
 *      A2MethodsUArray2 a2_vcs: UArray2 that contains VCS data.
 *      void *elem: pointer to element at col, row in a2_vcs.
 *      void *cl: closure pointer. In this function, contains output array
 *                of block average data and array to hold 4 VCS structs. 
 * Return:
 *      None
 * Expects:
 *      elem and a2_vcs to not be NULL
 * Notes:
 *      Raises CRE if:
 *      - a2, elem, cl are NULL
 *      - If memory is not allocated for block_data
 ****************************************************************************/
void vcs_to_blockavg_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl)
{
        assert(a2_vcs != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        Vcs_T vcs_data = (Vcs_T) elem;
        cl_T closure = (cl_T) cl;
        /* Load vcs data from elem onto VCS data array 
         * calculating index between 0 and 3 from col and row
         */
        int idx = 2 * (row % 2) + (col % 2);
        (closure->cells)[idx] = vcs_data;
        if ((col % 2 == 1) && (row % 2 == 1)) {
                Block_T block_data = malloc(sizeof(struct Block_avg));
                assert(block_data != NULL);
                block_data->a    = get_a(closure->cells);
                block_data->b    = get_b(closure->cells);
                block_data->c    = get_c(closure->cells);
                block_data->d    = get_d(closure->cells);
                block_data->pb_q = get_pb_q(closure->cells);
                block_data->pr_q = get_pr_q(closure->cells);
                *(Block_T) METHODS_P->at(closure->arr, (col + 1) / 2 - 1,
                                        (row + 1) / 2 - 1) = *block_data;
                free(block_data);
        }
}

/**********blockavg_to_vcs****************************************************
 *
 * Purpose:
 *      Converts a UArray2 with block data to a UArray2 with VCS data.
 * Parameters: 
 *      A2Methods_UArray2 a2_blk: UArray2 with block data.
 * Return: 
 *      A2Methods_UArray2 with VCS data that is twice the width and height of
 *      the input UArray2.
 * Expects:
 *      a2_blk to not be NULL
 * Notes:
 *      Raises CRE if:
 *      - a2_blk is NULL
 *      - memory is not allocated for a2_vcs.
 ****************************************************************************/
A2Methods_UArray2 blockavg_to_vcs(A2Methods_UArray2 a2_blk) 
{
        assert(a2_blk != NULL);
        int width = METHODS_P->width(a2_blk);
        int height = METHODS_P->height(a2_blk);
        A2Methods_UArray2 a2_vcs = METHODS_P->new(width * 2, height * 2,
                                              sizeof(struct Vcs));
        assert(a2_vcs != NULL);
        METHODS_P->map_default(a2_blk, blockavg_to_vcs_apply, a2_vcs);
        METHODS_P->free(&a2_blk);
        return a2_vcs;
}

/**********blockavg_to_vcs_apply**********************************************
 *
 * Purpose:
 *      apply function called by bloackavg_to_vcs. Converts a single block
 *      data struct and converts it into a 2x2 block of VCS data.
 * Parameters: 
 *      int col: column at which block data is in a2_blk
 *      int row: row at which block data is in a2_blk
 *      A2MethodsUArray2 a2_blk: UArray2 that contains block data.
 *      void *elem: pointer to element at col, row in a2_blk.
 *      void *cl: closure pointer. In this function, contains output array.
 * Return: 
 *      None
 * Expects:
 *      a2_blk, elem, and cl to not be NULL
 * Notes:
 *      Raises CRE if:
 *      - a2_blk, elem or closure are NULL
 *      - memory is not allocated for any element in the Vcs_T array.
 ****************************************************************************/
void blockavg_to_vcs_apply(int col, int row, A2Methods_UArray2 a2_blk, 
                           void *elem, void *cl)
{
        assert(a2_blk != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        A2Methods_UArray2 a2_vcs = (A2Methods_UArray2) cl;
        Block_T block_data = (Block_T) elem;
        Vcs_T vcs_1 = malloc(sizeof(*vcs_1));
        Vcs_T vcs_2 = malloc(sizeof(*vcs_2));
        Vcs_T vcs_3 = malloc(sizeof(*vcs_3));
        Vcs_T vcs_4 = malloc(sizeof(*vcs_4));
        assert(vcs_1 != NULL);
        assert(vcs_2 != NULL);
        assert(vcs_3 != NULL);
        assert(vcs_4 != NULL);
        vcs_1->y  = get_y1(block_data);
        vcs_2->y  = get_y2(block_data);
        vcs_3->y  = get_y3(block_data);
        vcs_4->y  = get_y4(block_data);
        Vcs_T vcs_arr[] = {vcs_1, vcs_2, vcs_3, vcs_4};
        for (int i = 0; i < NUM_CELLS; i++) {
                (vcs_arr[i])->pb = Arith40_chroma_of_index(block_data->pb_q);
                (vcs_arr[i])->pr = Arith40_chroma_of_index(block_data->pr_q);
                int c = col * 2 + i % 2;
                int r = row * 2 + i / 2;
                *(Vcs_T) METHODS_P->at(a2_vcs, c, r) = *vcs_arr[i];
                free(vcs_arr[i]);
        }
}