/*****************************************************************************
 *
 *                           vcs_blockavg_convert.h
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary: 
 *      - Contains declarations for functions that convert 2D arrays with
 *        vcs data to average block data and back.
 *      
 *      
 ****************************************************************************/
#include "a2blocked.h"

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
A2Methods_UArray2 vcs_to_blockavg(A2Methods_UArray2 a2_vcs);

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
A2Methods_UArray2 blockavg_to_vcs(A2Methods_UArray2 a2_b);