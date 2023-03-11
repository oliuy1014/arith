/*****************************************************************************
 *
 *                 image_pixmap_convert.c
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary:
 *      -Compression: Reads image file, extracts and returns Uarray2 pixmap
 *      -Decompression: Creates ppm using input Uarray2 as pixmap, writes
 *                      ppm to stdout
 *
 ****************************************************************************/
#include"image_pixmap_convert.h"
#include"assert.h"
#include"structs_and_constants.h"
#include <stdlib.h>

/* function declarations */
void copy_elem(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl);
A2Methods_UArray2 trim(A2Methods_UArray2 a2, int width, int height);

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
A2Methods_UArray2 image_to_pixmap(FILE *fp)
{
        assert(fp != NULL);
        Pnm_ppm image = Pnm_ppmread(fp, METHODS_P);
        int width = METHODS_P->width(image->pixels);
        int height = METHODS_P->height(image->pixels);
        assert(width >= MIN_WIDTH);
        assert(height >= MIN_HEIGHT);
        int even_w = width - (width % 2);
        int even_h = height - (height % 2);
        A2Methods_UArray2 a2_rgb = trim(image->pixels, even_w, even_h);
        Pnm_ppmfree(&image);
        return a2_rgb;
}

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
void pixmap_to_image(A2Methods_UArray2 a2_rgb)
{
        assert(a2_rgb != NULL);
        Pnm_ppm final_image = malloc(sizeof(*final_image));
        assert(final_image != NULL);
        final_image->pixels = (A2Methods_UArray2) a2_rgb;
        final_image->width = METHODS_P->width(a2_rgb);
        final_image->height = METHODS_P->height(a2_rgb);
        final_image->denominator = DENOMINATOR_D;
        final_image->methods = METHODS_P;
        Pnm_ppmwrite(stdout, final_image);
        Pnm_ppmfree(&final_image);
}

/*********copy_elem****************************************************
 *
 * Purpose:
 *      Copies elements from one Uarray2 to another
 * Parameters:
 *      int col, row: coordinates of current elem inside a2
 *      A2MethodsUArray2 a2: UArray2 being copied from
 *      void *elem: pointer to element at col, row in a2.
 *      void *cl: closure pointer, expected to point to the UArray2 to be
 *                populated with data copied from a2
 * Returns: 
 *      None
 * Expects:
 *      non-NULL elem and a2
 * Notes:
 *      raises CRE if a2_cw or elem are NULL
 ****************************************************************************/
void copy_elem(int col, int row, A2Methods_UArray2 a2, void *elem, void *cl)
{
        assert(a2 != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        (void) a2;
        A2Methods_UArray2 output_arr = *(A2Methods_UArray2*) cl;
        Pnm_rgb rgb = (Pnm_rgb) elem;
        *(Pnm_rgb) METHODS_P->at(output_arr, col, row) = *rgb;
}

/**********trim**************************************************
 *
 * Purpose:
 *      trims given a2 to a given width and height
 * Parameters: 
 *      - A2Methods_UArray2 a2: input UArray2 to be trimmed
 *      - int width, height: width and height for a2 to be trimmed to
 * Returns: 
 *      Trimmed UArray2
 * Expects:
 *      non-NULL input UArray2 with Pnm_rgb structs
 * Notes:
 *      Raises CRE if:
 *      - a2 is NULL
 *      - memory cannot be allocated for new UArray2
 ****************************************************************************/
A2Methods_UArray2 trim(A2Methods_UArray2 a2, int width, int height)
{
        assert(a2 != NULL);
        A2Methods_UArray2 a2_new = METHODS_P->new(width, height, 
                                                sizeof(struct Pnm_rgb));
        assert(a2_new != NULL);
        for (int c = 0; c < width; c++) {
                for (int r = 0; r < height; r++) {
                        Pnm_rgb elem = (Pnm_rgb) METHODS_P->at(a2, c, r);
                        *(Pnm_rgb) METHODS_P->at(a2_new, c, r) = *elem;
                }
        }
        return a2_new;
}
