#include <stdio.h>
#include <stdlib.h>
#include "blk_vs_cw.h"
#include "structs_and_constants.h"
#include "a2blocked.h"
#include "assert.h"
#include "mem.h"

#define CW_BITS  32
#define CW_FIELDS 6

typedef struct closure {
        A2Methods_UArray2 arr;
        fcoords coords[CW_FIELDS];
} *cl_T;

const int cw_widths[] = {A_W, BCD_W, BCD_W, BCD_W, PBR_W, PBR_W};

void SET_FORMAT(fcoords cw_coords[])
{
        unsigned curr_msb = CW_BITS;
        for (int i = 0; i < CW_FIELDS; i++) {
                cw_coords[i]->width = cw_widths[i];
                cw_coords[i]->lsb = curr_msb - cw_widths[i];
                curr_msb = curr_msb - cw_widths[i];
        }
}

void blk_to_cw_apply(int col, int row, A2Methods_UArray2 a2_blk, 
                  void *elem, void *cl);

void cw_to_blk_apply(int col, int row, A2Methods_UArray2 a2_cw, 
                     void *elem, void *cl);                      

A2Methods_UArray2 blk_to_cw(A2Methods_UArray2 a2_b)
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
        A2Methods_UArray2 a2_cw = METHODS_P->new(width, height, sizeof(uint64_t));
        closure->arr = a2_cw;
        METHODS_P->map_default(a2_b, blk_to_cw_apply, closure);
        for (int i = 0; i < CW_FIELDS; i++) {
                free(cw_format[i]);
        }
        // for (int col = 0; col < width; col++) {
        //         for (int row = 0; row < height; row++) {
        //                 uint32_t codeword = *(uint32_t*) METHODS_P->at(a2_cw, col, row);
        //                 printf("codeword: %u\n", codeword);
        //                 (void) codeword;
        //         }
        // }
        METHODS_P->free(&a2_b);
        free(closure);
        return a2_cw;
}

void blk_to_cw_apply(int col, int row, A2Methods_UArray2 a2_blk, 
                     void *elem, void *cl)       
{
        (void) a2_blk;
        Block_T blk_data = (Block_T) elem;
        uint64_t  a = (int) blk_data->a;
        int64_t   b = (int) blk_data->b;
        int64_t   c = (int) blk_data->c;
        int64_t   d = (int) blk_data->d;
        uint64_t pb = (int) blk_data->pb_q;
        uint64_t pr = (int) blk_data->pr_q;

        //fprintf(stderr, "a: %ld, b: %ld, c: %ld, d: %ld, pb: %ld, pr: %ld\t", a, b, c, d, pb, pr);
        uint32_t cw = 0;
        cl_T CL = (cl_T) cl;
        // fprintf(stderr, "a: %ld, width: %d\n", a, CL->coords[0]->width);
        cw = Bitpack_newu(cw, CL->coords[0]->width, CL->coords[0]->lsb, a);
        cw = Bitpack_news(cw, CL->coords[1]->width, CL->coords[1]->lsb, b);
        cw = Bitpack_news(cw, CL->coords[2]->width, CL->coords[2]->lsb, c);
        cw = Bitpack_news(cw, CL->coords[3]->width, CL->coords[3]->lsb, d);
        cw = Bitpack_newu(cw, CL->coords[4]->width, CL->coords[4]->lsb, pb);
        cw = Bitpack_newu(cw, CL->coords[5]->width, CL->coords[5]->lsb, pr);

        *(uint32_t*) METHODS_P->at(CL->arr, col, row) = (uint32_t) cw;

        uint32_t codeword = *(uint32_t*) METHODS_P->at(CL->arr, col, row);
        (void) codeword;
        // printf("codeword: %u\n", codeword);
}

A2Methods_UArray2 cw_to_blk(A2Methods_UArray2 a2_cw)
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
        METHODS_P->map_default(a2_cw, cw_to_blk_apply, closure);
        for (int i = 0; i < CW_FIELDS; i++) {
                free(cw_format[i]);
        }
        METHODS_P->free(&a2_cw);
        free(closure);
        return a2_b;
}

void cw_to_blk_apply(int col, int row, A2Methods_UArray2 a2_cw, 
                     void *elem, void *cl)       
{
        (void) a2_cw;
        uint32_t cw = *(uint32_t*) elem;
        cl_T CL = (cl_T) cl; 
        Block_T block_data = malloc(sizeof(struct Block_avg));
                             
        block_data->a    = Bitpack_getu(cw, CL->coords[0]->width, CL->coords[0]->lsb);
        block_data->b    = Bitpack_gets(cw, CL->coords[1]->width, CL->coords[1]->lsb);
        block_data->c    = Bitpack_gets(cw, CL->coords[2]->width, CL->coords[2]->lsb);
        block_data->d    = Bitpack_gets(cw, CL->coords[3]->width, CL->coords[3]->lsb);
        block_data->pb_q = Bitpack_getu(cw, CL->coords[4]->width, CL->coords[4]->lsb);
        block_data->pr_q = Bitpack_getu(cw, CL->coords[5]->width, CL->coords[5]->lsb);

        *(Block_T) METHODS_P->at(CL->arr, col, row) = *block_data;
        free(block_data);
} 