/*****************************************************************************
 *
 *                 read_write_codewords.h
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary: 
 *      Contains declarations of functions that can write a compressed image
 *      to a file from a UArray2, or read codewords from a compressed image
 *      file into a UArray2 
 *
 *      
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "a2blocked.h"

/**********write_codewords****************************************************
 *
 * Purpose:
 *      Wrties codewords to a file from a UArray2
 * Parameters: 
 *      A2Methods_UArray2 a2_cw: UArray2 containing codewords
 * Returns: 
 *      None
 * Expects:
 *      a2_cw to not be NULL
 * Notes:
 *      - Raises CRE if a2_cw is NULL
 *      - writes codewords and frees input array.
 ****************************************************************************/
void write_codewords(A2Methods_UArray2 a2_cw);

/**********read_codewords**************************************************
 *
 * Purpose:
 *      Reads a file of codewords and loads into a Uarray2.
 * Parameters: 
 *      -FILE* compressed_file: file pointer to file containing codewords
 * Returns: 
 *      Uarray2 of codewords
 * Expects:
 *      File pointer is non-NULL
 * Notes:
 *      Raises CRE if given NULL File pointer
 ****************************************************************************/
A2Methods_UArray2 read_codewords(FILE* compressed_file); 