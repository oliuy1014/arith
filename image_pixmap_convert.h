/*****************************************************************************
 *
 *                 image_pixmap_convert.h
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary:
 *      Contains declarations for functions that can read an image file into a
 *      Uarray2 pixmap or write a Ppm to stdout with a given Uarray2 pixmap
 *
 ****************************************************************************/
#include <stdio.h>
#include "a2plain.h"
#include "pnm.h"


/**********image_to_pixmap**************************************************
 *
 * Purpose:
 *      Reads in ppm from file, extracts and returns Uarray2 pixmap
 * Parameters: 
 *      FILE *fp: pointer to file expected to contain ppm image
 * Returns:
 *      Uarray2 containing pixmap Pnm_rgb data from input image
 * Expects:
 *      non-NULL file pointer
 * Notes:
 *      Raises CRE if fp is NULL
 *      Creates a new UArray2 with the original pixels trimmed to an even
 *      height and width (if necessary).
 *      Reads in and frees Pnm_ppm given from file pointer.
 ****************************************************************************/
A2Methods_UArray2 image_to_pixmap(FILE *image);

/**********pixmap_to_image**************************************************
 *
 * Purpose:
 *      Creates a Pnm_ppm with given Uarray2 as pixmap data, writes image
 *      to stdout
 * Parameters: 
 *      A2Methods_UArray2 a2_rgb: Uarray2 of Pnm_rgb structs
 * Returns:
 *      None
 * Expects:
 *      non-NULL input a2_rgb
 * Notes:
 *      - Raises CRE if a2_rgb is NULL or if memory cannot be allocated for
 *      new Pnm_ppm (final_image)
 *      - Creates Pnm_ppm from given pixels and frees after writing to stdout
 ****************************************************************************/
void pixmap_to_image(A2Methods_UArray2 a2_rgb);