#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "a2plain.h"
#include "uarray2.h"
#include "pnm.h"

void usage(char *progname);
FILE *open_file(char *filename, char *mode);


int main(int argc, char *argv[])
{
        if (argc != 3) {
                fprintf(stderr, "Incorrect number of arguments.");
                usage(argv[0]);
        }
        if ((strcmp(argv[1], "-") == 0 && (strcmp(argv[2], "-") == 0))) {
                fprintf(stderr, "At most one input filename can be stdin.\n");
                usage(argv[0]);
        }
        FILE *fp_1 = open_file(argv[1], "rb");
        FILE *fp_2 = open_file(argv[1], "rb");
        A2Methods_T methods = uarray2_methods_plain;
        Pnm_ppm img_1 = Pnm_ppmread(fp_1, methods);
        Pnm_ppm img_2 = Pnm_ppmread(fp_2, methods);
        fclose(fp_1);
        fclose(fp_2);

        A2Methods_UArray2 pix_1 = img_1->pixels;
        A2Methods_UArray2 pix_2 = img_2->pixels;

        int pix1w = methods->width(pix_1);
        int pix2w = methods->width(pix_2);
        int pix1h = methods->height(pix_1);
        int pix2h = methods->height(pix_2);

        printf("pix1h = %d, pix1w = %d, pix2h = %d, pix2w = %d\n", pix1h, pix1w, pix2h, pix2w);

        if (abs(methods->width(pix_1) - methods->width(pix_2)) > 1 || 
            abs(methods->height(pix_1) - methods->height(pix_2)) > 1) {
                fprintf(stderr, "Height or width of given images \
                        differ by more than 1 pixel.");
                printf("1\n");
        }
        
        /* make apply function to do the math with pixels
         * apply maps through pix_1, val = pixel inside pix_1
         * cl has to hold pix_2, val_2 = methods->at(pix_2, i, j)
         * and cl has to hold E value to be updated
         * apply has to go inside val->red, green, blue and 
         * edge case: different bounds of images? 
         */
        
        Pnm_ppmfree(&img_1);
        Pnm_ppmfree(&img_2);
}

void usage(char *progname)
{
        fprintf(stderr, "Usage: %s [filename] [filename]\n", progname);
        exit(1);
}

FILE *open_file(char *filename, char *mode) 
{
        assert(filename != NULL);
        assert(mode != NULL);
        FILE *returnfp;
        if (strcmp(filename, "-") == 0) {
                returnfp = stdin;
        } else {
                returnfp = fopen(filename, mode);
                if (returnfp == NULL) {
                        fprintf(stderr, "could not open file %s.\n", filename);
                        exit(1);
                }
        }
        return returnfp;
}