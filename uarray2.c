/**************************************************************
 *
 *                    uarray2.c
 *
 *      Assignment: Homework 2 -- iii
 *      Authors: Roman Short (rshort02), Oliver Uy (ouy01)
 *      Date: February 21st, 2023
 *     
 *      summary:
 *      Implementation of the uarray2 interface, using a 
 *      UArray_T to implement the 2d unboxed array as one long array.
 *      Implements the functions defined by uarray2.h
 *
 **************************************************************/

#include "uarray2.h"

#define T UArray2_T

/*
 * Struct T
 * The UArray2_T 2d unboxed array
 * width, height and size are integers, and the 
 * array itself is stored as a long 1d UArray_T array
 */
struct T {
        int width, height, size;
        UArray_T array;
};

/**********UArray2_new********
 *
 * Creates a new UArray2_T 2d unboxed array of bits
 * Inputs:
 *              int width: desired width of the 2d unboxed array
 *              int height: desired height of the 2d unboxed array
 *              int size: size of each element 
 * Return: a UArray2_T 2d unboxed array of width * height elements,
 *         all of given size, if paramaters are valid. 
 * Expects:
 *      int width and int height to be >= 0,
 *      int size to be > 0. 
 * Notes:
 *      space allocated for new_array is freed by UArray2_free
 *              May CRE if NEW fails.
 *      Will CRE if height or width are < 0, or if size < 1.
 ************************/
T UArray2_new(int width, int height, int size)
{     
        assert(width >= 0);
        assert(height >= 0);
        assert(size > 0);

        T new_array;
        NEW(new_array);

        new_array->width = width; 
        new_array->height = height;
        new_array->size = size;
        new_array->array = UArray_new(width * height, size);

        return new_array;
}

/**********UArray2_free********
 *
 * Frees a UArray2_T 2d unboxed array from memory
 * Inputs:
 *              T *UArray2: a pointer to a UArray2_T 2d unboxed array
 * Return: none
 * Expects:
 *      UArray2 and *UArray2 are non-NULL
 * Notes:
 *      Will CRE if UArray2 or *UArray2 are NULL
 ************************/
void UArray2_free(T *uarray2)
{
        assert(uarray2 != NULL && *uarray2 != NULL);

        UArray_free(&(*uarray2)->array);
        FREE(*uarray2);
}

/**********UArray2_width********
 *
 * Gets the width of a UArray2_T 2d unboxed array
 * Inputs:
 *              T UArray2: a UArray2_T 2d unboxed array
 * Return: the integer width of the UArray2_T 2d unboxed array
 * Expects:
 *      UArray2 is non-NULL
 * Notes:
 *      Will CRE if UArray2 is NULL
 ************************/
int UArray2_width(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

/**********UArray2_height********
 *
 * Gets the height of a UArray2_T 2d unboxed array
 * Inputs:
 *              T UArray2: a UArray2_T 2d unboxed array
 * Return: the integer height of the UArray2_T 2d unboxed array
 * Expects:
 *      UArray2 is non-NULL
 * Notes:
 *      Will CRE if UArray2 is NULL
 ************************/
int UArray2_height(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

/**********UArray2_size********
 *
 * Gets the size of a UArray2_T 2d unboxed array
 * Inputs:
 *              T UArray2: a UArray2_T 2d unboxed array
 * Return: the integer size of the UArray2_T 2d unboxed array
 * Expects:
 *      UArray2 is non-NULL
 * Notes:
 *      Will CRE if UArray2 is NULL
 ************************/
int UArray2_size(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->size;
}

/**********UArray2_at********
 *
 * Gets the pointer to the element in given UArray2_T,
 *      at index (col, row)
 * Inputs:
 *              T uarray2: a UArray2_T 2d unboxed array
 *              int col: column index to access
 *              int row: row index to access
 * Return: the integer value of the bit at col, row
 * Expects:
 *      uarray2 is non-NULL
 *      col is within bounds of provided UArray2_T
 *      row is within bounds of provided UArray2_T
 * Notes:
 *      Will CRE if uarray2 is NULL, col is out of bounds, 
 *      or row is out of bounds. 
 ************************/
void *UArray2_at(T uarray2, int col, int row)
{
        assert(uarray2 != NULL);
        assert(col >= 0 && col < uarray2->width);
        assert(row >= 0 && row < uarray2->height);
        /* math for determining location in 1d UArray */
        int idx = col + (row * uarray2->width);
        return UArray_at(uarray2->array, idx);
}

/**********UArray2_map_row_major********
 *
 * Calls apply function in row major order on every 
 * element within the provided UArray2_T 2d unboxed array
 * Inputs:
 *              T uarray2: a UArray2_T 2d unboxed array
 *              void apply: function to call for each element
 *              void *cl: closure pointer for use by client
 * Return: none
 * Expects:
 *      uarray2 is non-NULL
 * Notes:
 *      Will CRE if uarray2 is NULL.
 ************************/
void UArray2_map_row_major(T uarray2, void apply(int i, int j, T uarray2, 
                                                        void *val, void *cl),
                                                        void *cl)
{
        assert(uarray2 != NULL);

        for (int j = 0; j < uarray2->height; j++) {
                for (int i = 0; i < uarray2->width; i++) {
                        int idx = i + (j * uarray2->width);
                        void *current_p = UArray_at(uarray2->array, idx);
                        apply(i, j, uarray2, current_p, cl);
                }
        }
}

/**********UArray2_map_col_major********
 *
 * Calls apply function in column major order on every 
 * element within the provided UArray2_T 2d unboxed array
 * Inputs:
 *              T uarray2: a UArray2_T 2d unboxed array
 *              void apply: function to call for each element
 *              void *cl: closure pointer for use by client
 * Return: none
 * Expects:
 *      uarray2 is non-NULL
 * Notes:
 *      Will CRE if uarray2 is NULL.
 ************************/
void UArray2_map_col_major(T uarray2, void apply(int i, int j, T uarray2, 
                                                 void *val, void *cl), 
                                                 void *cl)
{
        assert(uarray2 != NULL);

        for (int i = 0; i < uarray2->width; i++) {
                for (int j = 0; j < uarray2->height; j++) {
                        int idx = i + (j * uarray2->width);
                        void *current_p = UArray_at(uarray2->array, idx);
                        apply(i, j, uarray2, current_p, cl);
                }
        }
}