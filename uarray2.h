/**************************************************************
 *
 *                    uarray2.h
 *
 *      Assignment: Homework 2 -- iii
 *      Authors: Roman Short (rshort02), Oliver Uy (ouy01)
 *      Date: February 21st, 2023
 *     
 *      summary:
 *      Interface for UArray2_T, a 2 dimensional unboxed array.
 *      Defines the functions for uarray2.c
 * 
 *
 **************************************************************/

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uarray.h"
#include <mem.h>

#define T UArray2_T
typedef struct T *T;

extern T UArray2_new(int width, int height, int size);
extern void UArray2_free(T *uarray2);

extern int UArray2_width(T uarray2);
extern int UArray2_height(T uarray2);
extern int UArray2_size(T uarray2);

extern void *UArray2_at(T uarray2, int col, int row);

extern void UArray2_map_col_major(T uarray2, void apply(int i, int j, 
                                                        T uarray2, void *val,
                                                        void *cl), void *cl);
extern void UArray2_map_row_major(T uarray2, void apply(int i, int j, 
                                                        T uarray2, void *val,
                                                        void *cl), void *cl);

#undef T
#endif

