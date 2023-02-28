/**************************************************************
 *
 *                    a2plain.c
 *
 *      Assignment: Homework 3 -- locality
 *      Authors: Roman Short (rshort02), Oliver Uy (ouy01)
 *      Date: February 21st, 2023
 *     
 *      summary:
 *      Implementation of the a2plain interface. Serves as one
 *      option for a2methods to process a 2d array, without
 *      blocks.
 *
 **************************************************************/

#include <string.h>

#include <a2plain.h>
#include "uarray2.h"

typedef A2Methods_UArray2 A2;

/**********new********
 *
 * Creates a new A2Methods_UArray2 plain 2d unboxed array 
 * Inputs:
 *              int width: desired width of the 2d unboxed array
 *              int height: desired height of the 2d unboxed array
 *              int size: size of each element 
 * Return: an A2Methods_Uarray2 2d unboxed array of width * height elements,
 *         all of given size, if paramaters are valid. 
 * Expects:
 *      int width and int height to be >= 0,
 *      int size to be > 0.
 * Notes:
 *      Uses functions defined in uarray2.h 
 *      for memory allocation and deallocation.
 *      Will CRE if height or width are < 0, or if size < 1.
 ************************/
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}

/**********new_with_blocksize********
 *
 * Creates a new UArray2_T 2d unboxed array of bits
 * Inputs:
 *              int width: desired width of the 2d unboxed array
 *              int height: desired height of the 2d unboxed array
 *              int size: desired size of each element. 
 *              int blocksize: size of each block, irrelevant for a2plain
 * Return: an A2Methods_UArray2 2d unboxed array of width * height elements,
 *         all of given size, if paramaters are valid. 
 * Expects:
 *      int width and int height to be >= 0,
 *      int size to be > 0. 
 * Notes:
 *      Uses functions defined in uarray2.h 
 *      for memory allocation and deallocation.
 *      Will CRE if height or width are < 0, or if size < 1.
 ************************/
static A2Methods_UArray2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        (void) blocksize;
        return UArray2_new(width, height, size);
}

/**********UArray2_free********
 *
 * Frees an A2Methods_Uarray2 2d unboxed array from memory
 * Inputs:
 *              A2 *array2p: a pointer to the array
 * Return: none
 * Expects:
 *      array2p and *array2p are non-NULL
 * Notes:
 *      deallocation and asserts are handled by 
 *      UArray2_free.
 *      Will CRE if UArray2 or *UArray2 are NULL
 ************************/
static void a2free(A2 *array2p)
{
        UArray2_free((UArray2_T *) array2p);
}

/**********width********
 *
 * Gets the width of an A2 2d unboxed array
 * Inputs:
 *              A2 array2: an A2 2d unboxed array
 * Return: the integer width of the A2
 * Expects:
 *      array2 is non-NULL
 * Notes:
 *      Will CRE if array2 is NULL, in UArray2_width. 
 ************************/
static int width(A2 array2)
{
        return(UArray2_width(array2));
}

/**********height********
 *
 * Gets the height of an A2 2d unboxed array
 * Inputs:
 *              A2 array2: an A2 2d unboxed array
 * Return: the integer height of the A2
 * Expects:
 *      array2 is non-NULL
 * Notes:
 *      Will CRE if array2 is NULL, in UArray2_height. 
 ************************/
static int height(A2 array2)
{
        return(UArray2_height(array2));
}

/**********size********
 *
 * Gets the size of an A2 2d unboxed array
 * Inputs:
 *              A2 array2: an A2 2d unboxed array
 * Return: the integer size of each element of the A2
 * Expects:
 *      array2 is non-NULL
 * Notes:
 *      Will CRE if array2 is NULL, in UArray2_size. 
 ************************/
static int size(A2 array2)
{
        return(UArray2_size(array2));
}

/**********blocksize********
 *
 * Gets the blocksize of an A2 2d unboxed array
 * Inputs:
 *              A2 array2: an A2 2d unboxed array
 * Return: the integer blocksize of the A2, which
 *         is always 1 as a2plain does not use blocks.
 * Expects:
 *      none
 * Notes:
 *      none
 ************************/
static int blocksize(A2 array2)
{
        (void) array2;
        return 1;
}

/**********at********
 *
 * Gets the pointer to the element in given A2,
 *      at index (i, j)
 * Inputs:
 *              A2 array2: an A2 2d unboxed array
 *              int i: column index to access
 *              int j: row index to access
 * Return: the pointer to the A2Methods_Object
 * Expects:
 *      array2 is non-NULL
 *      i is within bounds of provided A2
 *      j is within bounds of provided A2
 * Notes:
 *      Will CRE if array2 is NULL, i is out of bounds, 
 *      or j is out of bounds, all handled 
 *      by UArray2_at. 
 ************************/
static A2Methods_Object *at(A2 array2, int i, int j)
{
        return UArray2_at(array2, i, j);
}

/**********UArray2_applyfun********
 *
 * typedefs an apply function to be passed into map.
 * Inputs:
 *              int i: column index to access
 *              int j: row index to access
 *              UArray2_T array2: a UArray2_T 2d unboxed array
 *              void *elem: pointer to current element
 *              void *cl: closure pointer
 * Return: none
 * Expects:
 *      none
 * Notes:
 *      none
 ************************/
typedef void UArray2_applyfun(int i, int j, UArray2_T array2, 
                              void *elem, void *cl);

/**********map_row_major********
 *
 * Calls apply function in row major order on every 
 * element within the provided A2Methods_UArray2 2d unboxed array
 * Inputs:
 *              A2Methods_Uarray2 uarray2: an A2 2d unboxed array
 *              A2Methods_applyfun apply: function to call for each element
 *              void *cl: closure pointer for use by client
 * Return: none
 * Expects:
 *      uarray2 is non-NULL.
 * Notes:
 *      CRE for uarray2 being NULL is handled by uarray2.c
 ************************/
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_row_major(uarray2, (UArray2_applyfun*)apply, cl);
}

/**********map_col_major********
 *
 * Calls apply function in row major order on every 
 * element within the provided A2Methods_UArray2 2d unboxed array
 * Inputs:
 *              A2Methods_Uarray2 uarray2: an A2 2d unboxed array
 *              A2Methods_applyfun apply: function to call for each element
 *              void *cl: closure pointer for use by client
 * Return: none
 * Expects:
 *      uarray2 is non-NULL.
 * Notes:
 *      CRE for uarray2 being NULL is handled by uarray2.c
 ************************/
static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_col_major(uarray2, (UArray2_applyfun*)apply, cl);
}

/*
 * struct small_closure
 * contains an A2Methods_smallapplyfun, and a void closure pointer.
 */
struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void                    *cl;
};

/**********apply_small********
 *
 * small apply function for a UArray2_T. calls apply on elem.
 * Inputs:
 *              int i: unused column index
 *              int j: unused row index
 *              UArray2_T array2: an unused UArray2_T 2d unboxed array
 *              void *elem: pointer to current element
 *              void *cl: closure pointer
 * Return: none
 * Expects:
 *      none
 * Notes:
 *      none
 ************************/
static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl);
}

/**********small_map_row_major********
 *
 * Calls small apply function in row major order on every 
 * element within the provided A2Methods_UArray2 2d unboxed array
 * Inputs:
 *              A2Methods_Uarray2 a2: an A2 2d unboxed array
 *              A2Methods_smallapplyfun apply: small apply function
 *                      for each element
 *              void *cl: closure pointer for use by client
 * Return: none
 * Expects:
 *      uarray2 is non-NULL.
 * Notes:
 *      CRE for uarray2 being NULL is handled by uarray2.c
 ************************/
static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}

/**********small_map_col_major********
 *
 * Calls small apply function in column major order on every 
 * element within the provided A2Methods_UArray2 2d unboxed array
 * Inputs:
 *              A2Methods_Uarray2 a2: an A2 2d unboxed array
 *              A2Methods_smallapplyfun apply: small apply function
 *                      for each element
 *              void *cl: closure pointer for use by client
 * Return: none
 * Expects:
 *      uarray2 is non-NULL.
 * Notes:
 *      CRE for uarray2 being NULL is handled by uarray2.c
 ************************/
static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}

/*
 * struct A2Methods_T uarray2_methods_plain_struct
 * contains the A2 functions useable by an a2plain instance.
 */
static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        map_row_major,
        map_col_major,
        NULL,
        map_row_major,
        small_map_row_major,
        small_map_col_major,
        NULL,
        small_map_row_major,
};

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
