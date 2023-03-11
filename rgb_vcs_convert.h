/*****************************************************************************
 *
 *                 rgb_vcs_convert.h
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary: 
 *      Contains declarations for functions that can convert a UArray2
 *      from RGB data to VCS data and back
 *
 ****************************************************************************/
#include "a2blocked.h"
#include "a2plain.h"

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
A2Methods_UArray2 rgb_to_vcs(A2Methods_UArray2 a2_rgb);

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
A2Methods_UArray2 vcs_to_rgb(A2Methods_UArray2 a2_vcs);