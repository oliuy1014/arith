#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "a2plain.h"
#include "uarray2.h"
#include "pnm.h"

void usage(char *progname);
FILE *open_file(char *filename, char *mode);
double diff_score(Pnm_ppm img_1, Pnm_ppm img_2, A2Methods_T methods);
void set_files(FILE **fp_1, FILE **fp_2, char *filename_1, char *filename_2);
int min(int a, int b);
void trim_file(A2Methods_UArray2 pixmap, int w, int h, A2Methods_T methods);
void check_dimensions(Pnm_ppm img_1, Pnm_ppm img_2);

int main(int argc, char *argv[])
{
        if (argc != 3) {
                fprintf(stderr, "Invalid number of arguments.\n");
                usage(argv[0]);
        }
        if ((strcmp(argv[1], "-") == 0 && (strcmp(argv[2], "-") == 0))) {
                fprintf(stderr, "At most one input filename can be stdin.\n");
                usage(argv[0]);
        }
        FILE *fp_1 = NULL;
        FILE *fp_2 = NULL;
        set_files(&fp_1, &fp_2, argv[1], argv[2]);
        A2Methods_T methods = uarray2_methods_plain;
        Pnm_ppm img_1 = Pnm_ppmread(fp_1, methods);
        Pnm_ppm img_2 = Pnm_ppmread(fp_2, methods);
        fclose(fp_1);
        fclose(fp_2);
        check_dimensions(img_1, img_2);
        double diff = diff_score(img_1, img_2, methods);
        printf("%f\n", diff);
        Pnm_ppmfree(&img_1);
        Pnm_ppmfree(&img_2);
        return 0;
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

double diff_score(Pnm_ppm img_1, Pnm_ppm img_2, A2Methods_T methods)
{
        assert(img_1 != NULL);
        assert(img_2 != NULL);
        assert(methods != NULL);
        int width = min(img_1->width, img_2->width);
        int height = min(img_1->height, img_2->height);
        double denom_1 = (double) img_1->denominator;
        double denom_2 = (double) img_2->denominator;
        double diff = 0;
        for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                        Pnm_rgb p1 = methods->at(img_1->pixels, i, j);
                        Pnm_rgb p2 = methods->at(img_2->pixels, i, j);
                        double r = (double) (p1->red / denom_1) - 
                                   (double) (p2->red / denom_2);
                        double g = (double) (p1->green / denom_1) - 
                                   (double) (p2->green / denom_2);
                        double b = (double) (p1->blue / denom_1) - 
                                   (double) (p2->blue / denom_2);
                        diff += ((r * r) + (g * g) + (b * b));
                }
        }
        double ret = sqrt(diff / (3 * width * height));
        return ret;
}

void set_files(FILE **fp_1, FILE **fp_2, char *filename_1, char *filename_2) 
{
        if (strcmp(filename_1, "-") == 0) {
                *fp_1 = stdin;
        } else {
                *fp_1 = open_file(filename_1, "rb");
        }
        if (strcmp(filename_2, "-") == 0) {
                *fp_2 = stdin;
        } else {
                *fp_2 = open_file(filename_2, "rb");
        }
}

int min(int a, int b) 
{
        return a < b ? a : b;
}

void check_dimensions(Pnm_ppm img_1, Pnm_ppm img_2)
{
        int width_diff = abs((int) img_1->width - 
                             (int) img_2->width);
        int height_diff = abs((int) img_1->height - 
                              (int) img_2->height);
        if (width_diff > 1 || height_diff > 1) {
                fprintf(stderr, "Width or height of files differ"
                                " by more than 1 pixel\n");
                printf("1.0\n");
                exit(0);
        }
}