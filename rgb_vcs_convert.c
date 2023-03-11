/*****************************************************************************
 *
 *                 rgb_vcs_convert.c
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary: 
 *      - Compression: Converts UArray2 with RGB data to UArray2 with VCS data
 *      - Decompression: Converts UArray2 with VCS data to UArray2 with RGB
 *                       data.
 *      
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"
#include "pnm.h"
#include "rgb_vcs_convert.h"
#include "structs_and_constants.h"
#include "mem.h"

#define BLOCKSIZE 2

/*********function declarations and global variables*************************/
void rgb_to_vcs_apply(int col, int row, A2Methods_UArray2 a2_rgb,
                void *elem, void *cl);

void vcs_to_rgb_apply(int col, int row, A2Methods_UArray2 a2_vcs,
                      void *elem, void *cl);

int clamp_to_int(float input, int max, int min);
float clamp_to_float(float input, float max, float min);

/**********rgb_to_vcs*********************************************************
 *
 * Purpose:
 *      converts a UArray2 of RGB data to a UArray2 of VCS data.
 * Parameters: 
 *      A2Methods_UArray2 a2_rgb: UArray2 with RGB data
 * Return: 
 *      A2Methods_UArray2 with VCS data
 * Expects:
 *      a2_rgb to not be NULL
 * Notes:
 *      Raises CRE if:
 *      - a2_rgb is NULL
 *      - memory is not allocated for a2_vcs
 *      The input RGB UArray2 is freed after use.
 ****************************************************************************/
A2Methods_UArray2 rgb_to_vcs(A2Methods_UArray2 a2_rgb)
{
        assert(a2_rgb != NULL);
        int width = METHODS_P->width(a2_rgb);
        int height = METHODS_P->height(a2_rgb);
        A2Methods_UArray2 a2_vcs = METHODS_B->new_with_blocksize(width, height, 
                                                          sizeof(struct Vcs),
                                                                  BLOCKSIZE);
        assert(a2_vcs != NULL);
        METHODS_P->map_default(a2_rgb, rgb_to_vcs_apply, a2_vcs);
        METHODS_P->free(&a2_rgb);
        return a2_vcs;
}

/**********rbg_to_vcs_apply***************************************************
 *
 * Purpose:
 *      apply function called by rgb_to_vcs. Takes a single Pnm_rgb struct and
 *      computes VCS values which it loads onto the output UArray2.
 * Parameters: 
 *      int col: column in a2_rgb where the Pnm_rgb struct is.
 *      int row: row in a2_rgb where the Pnm_rgb struct is.
 *      A2MethodsUArray2 a2_rgb: Input array with RGB data.
 *      void *elem: pointer to element at col, row in a2_rgb.
 *      void *cl: closure pointer. Used as the output UArray2 in this function
 * Return: 
 *      None
 * Expects:
 *      a2_rgb, elem and cl to not be NULL
 * Notes:
 *      Raises CRE if:
 *      - a2_rgb, elem or cl are not NULL.
 *      - Memory is not allocated for vcs_data.
 ****************************************************************************/
void rgb_to_vcs_apply(int col, int row, A2Methods_UArray2 a2_rgb,
                      void *elem, void *cl)
{
        assert(a2_rgb != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2_rgb;

        /* read in array from cl */
        A2Methods_UArray2 a2_vcs = (A2Methods_UArray2) cl;
        assert(a2_vcs != NULL);
        /* initialize struct for rgb int data */
        Pnm_rgb int_data = (Pnm_rgb) elem;
        Vcs_T vcs_data = malloc(sizeof(struct Vcs));
        assert(vcs_data != NULL);
        float d = DENOMINATOR_C;

        float r = int_data->red   / d;
        float g = int_data->green / d;
        float b = int_data->blue  / d;
        
        /* convert float values to vcs */
        vcs_data->y  =  clamp_to_float(
                        (0.299    * r +
                        0.587     * g +
                        0.114     * b), 1, 0);

        vcs_data->pb = clamp_to_float(
                        (-0.168736 * r -
                        0.331264   * g +
                        0.5        * b), 0.5, -0.5);

        vcs_data->pr =  clamp_to_float(
                        (0.5      * r -
                        0.418688  * g -
                        0.081312  * b), 0.5, -0.5);
        *(Vcs_T) METHODS_B->at(a2_vcs, col, row) = *vcs_data;
        free(vcs_data);
}

/**********vcs_to_rgb*********************************************************
 *
 * Purpose:
 *      Converts a UArray2 with VCS data to a UArray2 with RGB data.
 * Parameters: 
 *      A2Methods_UArray2 a2_vcs: UArray2 with VCS data.
 * Return: 
 *      A2Methods_UArray2 with RGB data.
 * Expects:
 *      a2_vcs to not be NULL
 * Notes:
 *      Raises CRE if:
 *      - a2_vcs is NULL
 *      - Memory is not allocated for a2_rgb
 ****************************************************************************/
A2Methods_UArray2 vcs_to_rgb(A2Methods_UArray2 a2_vcs)
{
        assert(a2_vcs != NULL);
        int width = METHODS_P->width(a2_vcs);
        int height = METHODS_P->height(a2_vcs);
        A2Methods_UArray2 a2_rgb = METHODS_P->new(width, height,
                                                  sizeof(struct Pnm_rgb));
        assert(a2_rgb != NULL);
        METHODS_P->map_default(a2_vcs, vcs_to_rgb_apply, a2_rgb);
        METHODS_P->free(&a2_vcs);
        return a2_rgb;
}

/**********vcs_to_rgb_apply*****************************************
 *
 * Purpose:
 *      Apply function to be called by vcs_to_rgb. Converts VCS data for a
 *      pixel to a Pnm_rgb struct and loads the rgb pixel to the UArray2
 *      expected to be passed in the closure.
 * Parameters: 
 *      int col, row: coordinates of elem in a2_vcs being mapped through
 *      A2MethodsUArray2 a2_vcs: UArray2 of VCS data being mapped through
 *      void *elem: pointer to element in a2_vcs at col, row
 *      void *cl: closure pointer expected to contain UArray2 of rgb data
 * Returns: 
 *      None
 * Expects:
 *      non-NULL input a2_vcs, elem, cl
 * Notes:
 *      Raises CRE if given NULL input a2_vcs, elem, or cl pointer
 ****************************************************************************/
void vcs_to_rgb_apply(int col, int row, A2Methods_UArray2 a2_vcs, 
                  void *elem, void *cl)
{
        assert(a2_vcs != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        A2Methods_UArray2 a2_rgb = (A2Methods_UArray2) cl;
        Vcs_T vcs_data = (Vcs_T) elem;
        Pnm_rgb int_data;
        NEW(int_data);
        int d = DENOMINATOR_D;
        float y  = vcs_data->y;
        float pb = vcs_data->pb;
        float pr = vcs_data->pr;
        int_data->red   = clamp_to_int((1.0   * y  + 
                                        0.0   * pb +
                                        1.402 * pr) * d, d, 0);
        int_data->green = clamp_to_int((1.0      * y  - 
                                        0.344136 * pb - 
                                        0.714136 * pr) * d, d, 0);
        int_data->blue  = clamp_to_int((1.0      * y  +
                                        1.772    * pb +
                                        0.0      * pr) * d, d, 0);
        *(Pnm_rgb) METHODS_P->at(a2_rgb, col, row) = *int_data;
        free(int_data);
}

/*********clamp_to_int********************************************************
 *
 * Purpose:
 *      Clamps given value at desired maximum and minimum (returns minimim
 *      if value is less than minimum, returns maximum if value is greater
 *      than maximum). 
 * Parameters: 
 *      - float input: number being clamped
 *      - int max, min: values at which input is being clamped
 * Returns: 
 *      Clamped value as integer
 * Expects:
 *      None
 * Notes:
 *      None
 ****************************************************************************/
int clamp_to_int(float input, int max, int min)
{
        input = (input <= max) ? input : max;
        input = (input >= min) ? input : min;
        return (int)roundf(input);
}

/*********clamp_to_float******************************************************
 *
 * Purpose:
 *      Clamps given value at desired maximum and minimum (returns minimim
 *      if value is less than minimum, returns maximum if value is greater
 *      than maximum). 
 * Parameters: 
 *      - float input: number being clamped
 *      - int max, min: values at which input is being clamped
 * Returns: 
 *      Clamped value as float
 * Expects:
 *      None
 * Notes:
 *      None
 ****************************************************************************/
float clamp_to_float(float input, float max, float min)
{
        input = (input <= max) ? input : max;
        input = (input >= min) ? input : min;
        return input;
}