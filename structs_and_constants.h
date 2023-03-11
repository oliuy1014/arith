/*****************************************************************************
 *
 *                 structs_and_constants.h
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary: Contains structs and constants used by multiple modules in
 *      compress40 for compression and decompression of images.
 *
 *      
 *
 ****************************************************************************/
#ifndef _STRUCTS_AND_CONSTS_
#define _STRUCTS_AND_CONSTS_
#include "a2blocked.h"
#include "a2plain.h"

/**********struct definitions************************************************/

/* Vcs struct
 *
 * Purpose: stores data for a pixel in video color space
 * Members:
 *      - float y: luminance/brightness
 *      - float pb: blue chroma component
 *      - float pr: red chroma component
 */
typedef struct Vcs {
        float y, pb, pr;
} *Vcs_T;

/* Block_avg struct
 *
 * Purpose: stores quantized data for a 2x2 block of pixels
 * Members:
 *      - unsigned a: average luminance
 *      - int b, c, d: gradients of luminance across different axes of block
 *      - unsigned pb_q, pr_q: index of average blue and red chroma components
 */
typedef struct Block_avg {
        unsigned a, pb_q, pr_q;
        int b, c, d;
} *Block_T;

/**********global constants************************************************/

#define METHODS_P uarray2_methods_plain
#define METHODS_B uarray2_methods_blocked
#define DENOMINATOR_C 255
#define DENOMINATOR_D 255
#define A_W 9
#define BCD_W 5
#define PBR_W 4
#define A_SCALE 511
#define BCD_SCALE 50
#define MIN_WIDTH 2
#define MIN_HEIGHT 2
#endif