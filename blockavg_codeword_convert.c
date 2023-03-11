/*****************************************************************************
 *
 *                 blockavg_codeword_convert.c
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary:
 *      Compression: Converts a Uarray2 of with blockavg struct color data
 *                    to a Uarray2 of 32-bit codewords.
 *
 *      Decompression: Converts a Uarray2 of with 32-bit codewords to a
 *                      Uarray2 of blockavg struct color data.
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "blockavg_codeword_convert.h"
#include "structs_and_constants.h"
#include "a2blocked.h"
#include "assert.h"
#include "mem.h"

#define CW_BITS  32
#define CW_FIELDS 6

/**********struct definitions************************************************/

/* field_coords struct
 *
 * Purpose: Stores elements to place a field inside a codeword.
 *
 * Members:
 *      - int w: width of field in codeword
 *      - int lsb: location of least significant bit in codeword
 */
typedef struct field_coords {
        int w, lsb;
} *fcoords;

/* closure struct
 *
 * Purpose: Stores elements needed by later apply functions (passed in as
 *          cl pointer)
 *
 * Members:
 *      - A2Methods_UArray2 arr: UArray2 to be populated by function
 *      - fcoords coords[CW_FIELDS]: array of fcoords structs for each
 *        field of the codeword
 */
typedef struct closure {
        A2Methods_UArray2 arr;
        fcoords coords[CW_FIELDS];
} *cl_T;

/**********global codeword format********************************************/
/* Contains widths of all fields in desired order for codeword format */
const int cw_widths[] = {A_W, BCD_W, BCD_W, BCD_W, PBR_W, PBR_W};

/**********function declarations*********************************************/
void SET_FORMAT(fcoords cw_coords[]);
void blockavg_to_codeword_apply(int col, int row, A2Methods_UArray2 a2_blk,
                                void *elem, void *cl);
void codeword_to_blockavg_apply(int col, int row, A2Methods_UArray2 a2_cw,
                                void *elem, void *cl);                      

/**********blockavg_to_codeword***********************************************
 *
 * Purpose:
 *      Bitpacks all Block_T structs into 32-bit codewords and loads
 *      these codewords onto a new array
 * Parameters: 
 *      A2Methods_UArray2 a2_b: Input array containing Block_Ts.
 * Return: 
 *      Returns an A2Methods_UArray2 populated with codewords.
 * Expects:
 *      Expects a2_b to not be NULL.
 * Notes:
 *      Raises CRE if a2_b is NULL.
 *      Formats the codeword according to the given cw_format.
 *      Frees the input a2_b
 ****************************************************************************/
A2Methods_UArray2 blockavg_to_codeword(A2Methods_UArray2 a2_b)
{
        assert(a2_b != NULL);
        fcoords a  = malloc(sizeof(*a));
        fcoords b  = malloc(sizeof(*b));
        fcoords c  = malloc(sizeof(*c));
        fcoords d  = malloc(sizeof(*d));
        fcoords pb = malloc(sizeof(*pb));
        fcoords pr = malloc(sizeof(*pr));
        fcoords cw_format[CW_FIELDS] = {a, b, c, d, pb, pr};

        SET_FORMAT(cw_format);
        cl_T closure = malloc(sizeof(*closure));
        for (int i = 0; i < CW_FIELDS; i++) {
                closure->coords[i] = cw_format[i];
        }
        int width = METHODS_P->width(a2_b);
        int height = METHODS_P->height(a2_b);
        A2Methods_UArray2 a2_cw = METHODS_P->new(width, height,
                                                 sizeof(uint64_t));
        closure->arr = a2_cw;
        METHODS_P->map_default(a2_b, blockavg_to_codeword_apply, closure);
        for (int i = 0; i < CW_FIELDS; i++) {
                free(cw_format[i]);
        }
        METHODS_P->free(&a2_b);
        free(closure);
        return a2_cw;
}

/**********blockavg_to_codeword_apply*****************************************
 *
 * Purpose:
 *      apply function called by blockavg_to_codeword. Bitpacks the members of 
        a single Block_T into a 32-bit codeword and loads onto an output array
 * Parameters: 
 *      int col: column at which the block average data is in a2_blk. 
 *      int row: row at which the block average data is in a2_blk. 
 *      A2MethodsUArray2 a2_blk: UArray2 containing block average data.
 *      void *elem: pointer to element at col, row in a2_blk.
 *      void *cl: closure pointer. In this function, it contains the output
                  UArray2 with codewords and an array containing the format
                  of the codeword.
 * Return: 
 *      None
 * Expects:
 *      elem, cl and a2_blk to not be NULL
 * Notes:
 *      raises CRE if elem, cl or a2_blk are NULL
 ****************************************************************************/
void blockavg_to_codeword_apply(int col, int row, A2Methods_UArray2 a2_blk, 
                     void *elem, void *cl)       
{
        assert(a2_blk != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        Block_T blk_data = (Block_T) elem;
        uint64_t  a = (int) blk_data->a;
        int64_t   b = (int) blk_data->b;
        int64_t   c = (int) blk_data->c;
        int64_t   d = (int) blk_data->d;
        uint64_t pb = (int) blk_data->pb_q;
        uint64_t pr = (int) blk_data->pr_q;
        uint32_t cw = 0;
        cl_T CL = (cl_T) cl;
        cw = Bitpack_newu(cw, CL->coords[0]->w, CL->coords[0]->lsb, a);
        cw = Bitpack_news(cw, CL->coords[1]->w, CL->coords[1]->lsb, b);
        cw = Bitpack_news(cw, CL->coords[2]->w, CL->coords[2]->lsb, c);
        cw = Bitpack_news(cw, CL->coords[3]->w, CL->coords[3]->lsb, d);
        cw = Bitpack_newu(cw, CL->coords[4]->w, CL->coords[4]->lsb, pb);
        cw = Bitpack_newu(cw, CL->coords[5]->w, CL->coords[5]->lsb, pr);
        *(uint32_t*) METHODS_P->at(CL->arr, col, row) = (uint32_t) cw;
        uint32_t codeword = *(uint32_t*) METHODS_P->at(CL->arr, col, row);
        (void) codeword;
}

/**********codeword_to_blockavg***********************************************
 *
 * Purpose:
 *      Unpacks codewords from a UArray2 and converts to block average data
 *      then loads this data onto a new UArray2.
 * Parameters: 
 *      A2Methods_UArray2 a2_cw: UArray2 containing codewords.
 * Return: 
 *      A2Methods_UArray2 containing block average data.
 * Expects:
 *      a2_cw to not be NULL
 * Notes:
 *      raises CRE if a2_cw is NULL
 *      Frees the input a2_cw UArray2
 ****************************************************************************/
A2Methods_UArray2 codeword_to_blockavg(A2Methods_UArray2 a2_cw)
{
        assert(a2_cw != NULL);
        fcoords a  = malloc(sizeof(*a));
        fcoords b  = malloc(sizeof(*b));
        fcoords c  = malloc(sizeof(*c));
        fcoords d  = malloc(sizeof(*d));
        fcoords pb = malloc(sizeof(*pb));
        fcoords pr = malloc(sizeof(*pr));
        fcoords cw_format[CW_FIELDS] = {a, b, c, d, pb, pr};

        SET_FORMAT(cw_format);
        cl_T closure = malloc(sizeof(*closure));
        for (int i = 0; i < 6; i++) {
                closure->coords[i] = cw_format[i];
        }
        int width = METHODS_P->width(a2_cw);
        int height = METHODS_P->height(a2_cw);
        A2Methods_UArray2 a2_b = METHODS_P->new(width, height,
                                                sizeof(struct Block_avg));
        closure->arr = a2_b;
        METHODS_P->map_default(a2_cw, codeword_to_blockavg_apply, closure);
        for (int i = 0; i < CW_FIELDS; i++) {
                free(cw_format[i]);
        }
        METHODS_P->free(&a2_cw);
        free(closure);
        return a2_b;
}

/*********codeword_to_blockavg_apply******************************************
 *
 * Purpose:
 *      apply function called by codewword_to_blockavg. Unpacks a single
 *      codeword and converts to block average data.
 * Parameters:
 *      int col: column at which codeword is in a2_cw
 *      int row: row at which codeword is in a2_cw
 *      A2MethodsUArray2 a2_cw: UArray2 that contains 32-bit codewords
 *      void *elem: pointer to element at col, row in a2_cw.
 *      void *cl: closure pointer. In this function, contains output array
                  of block average data and format of the codeword.
 * Return: 
 *      None
 * Expects:
 *      elem, cl and a2_cw to not be NULL
 * Notes:
 *      raises CRE if a2_cw, cl or elem are NULL
 ****************************************************************************/
void codeword_to_blockavg_apply(int col, int row, A2Methods_UArray2 a2_cw, 
                     void *elem, void *cl)       
{ 
        assert(a2_cw != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        uint32_t cw = *(uint32_t*) elem;
        cl_T CL = (cl_T) cl; 
        Block_T block_data = malloc(sizeof(struct Block_avg));
                             
        block_data->a = Bitpack_getu(cw, CL->coords[0]->w, CL->coords[0]->lsb);
        block_data->b = Bitpack_gets(cw, CL->coords[1]->w, CL->coords[1]->lsb);
        block_data->c = Bitpack_gets(cw, CL->coords[2]->w, CL->coords[2]->lsb);
        block_data->d = Bitpack_gets(cw, CL->coords[3]->w, CL->coords[3]->lsb);
        block_data->pb_q = Bitpack_getu(cw, CL->coords[4]->w,
                                        CL->coords[4]->lsb);
        block_data->pr_q = Bitpack_getu(cw, CL->coords[5]->w,
                                        CL->coords[5]->lsb);

        *(Block_T) METHODS_P->at(CL->arr, col, row) = *block_data;
        free(block_data);
}

/**********SET_FORMAT*********************************************************
 *
 * Purpose:
 *      sets format for codeword from the cw_widths constant.
 * Parameters: 
 *      fcoords cw_coords[]: array of fcoords that make up the format.
 * Return: 
 *      none
 * Expects:
 *      all elements in cw_coords to not be NULL
 * Notes:
 *      cw_coords must have memory allocated for it before being passed into
 *      this function.
 *      
 ****************************************************************************/
void SET_FORMAT(fcoords cw_coords[])
{
        unsigned curr_msb = CW_BITS;
        for (int i = 0; i < CW_FIELDS; i++) {
                assert(cw_coords[i] != NULL);
                cw_coords[i]->w = cw_widths[i];
                cw_coords[i]->lsb = curr_msb - cw_widths[i];
                curr_msb = curr_msb - cw_widths[i];
        }
}