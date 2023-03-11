/*****************************************************************************
 *
 *                 blockavg_codeword_convert.h
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary:
 *      Contains declarations for functions that can convert a UArray2 of
 *      block average structs to a UArray2 of 32-int codewords and back
 *
 ****************************************************************************/
#include "bitpack.h"
#include "a2blocked.h"

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
A2Methods_UArray2 blockavg_to_codeword(A2Methods_UArray2 a2_b);

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
A2Methods_UArray2 codeword_to_blockavg(A2Methods_UArray2 a2_cw);