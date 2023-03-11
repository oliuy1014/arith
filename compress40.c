/**************************************************************
 *
 *                compress40.c 
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      summary:
 *      TODO
 *      
 *
 **************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include "compress40.h"
#include "image_pixmap_convert.h"
#include "rgb_vcs_convert.h"
#include "vcs_blockavg_convert.h"
#include "blockavg_codeword_convert.h"
#include "read_write_codewords.h"
#include "pnm.h"
#include "a2blocked.h"
#include "a2plain.h"

/**********compress40********************************************************

 *
 * Purpose:
 *      Compresses a given image file, writes compressed file to stdout
 * Parameters: 
 *      FILE *input: input file pointer to image
 * Returns: 
 *      None
 * Expects:
 *      Input file to contain ppm image
 * Notes:
 *      None
 ****************************************************************************/
void compress40(FILE *input)
{
        A2Methods_UArray2 a2_rgb   = image_to_pixmap(input);
        A2Methods_UArray2 a2_vcs   = rgb_to_vcs(a2_rgb);
        A2Methods_UArray2 a2_blks  = vcs_to_blockavg(a2_vcs);
        A2Methods_UArray2 a2_cw    = blockavg_to_codeword(a2_blks);
        write_codewords(a2_cw);
}

/*********decompress40********************************************************
 *
 * Purpose:
 *      Decompresses a given compressed image file, writes decompressed 
 *      image to stdout
 * Parameters: 
 *      FILE *input: input file pointer to compressed image
 * Returns: 
 *      None
 * Expects:
 *      Input file to contain compressed ppm image with appropriate codewords
 * Notes:
 *      None
 ****************************************************************************/
void decompress40(FILE *input)
{
        A2Methods_UArray2 a2_cw   = read_codewords(input);
        A2Methods_UArray2 a2_blks = codeword_to_blockavg(a2_cw);
        A2Methods_UArray2 a2_vcs  = blockavg_to_vcs(a2_blks);
        A2Methods_UArray2 a2_rgb  = vcs_to_rgb(a2_vcs);
        pixmap_to_image(a2_rgb);
}