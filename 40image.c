/*****************************************************************************
 *
 *                     40image.c
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary:
 *      Compression:
 *      - Compresses a given ppm image file and writes the compressed file of
          codewords to stdout.
 *      Decompression:
 *      - Decompresses a given compressed image file and writes the
 *        decompressed image to stdout.
 *      
 *
 ****************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"


/*********function declarations*************************/
static void (*compress_or_decompress)(FILE *input) = compress40;

/**********main function******************************************************
 *
 * Purpose:
 *      main driver for 40image program, reads input and either compresses
 *      an image or decompresses a compressed image file.
 * Parameters: 
 *      int argc: number of arguments given
 *      char *argv[]: array of arguments given
 * Returns: 
 *      integer 0 or 1 signifying exit success or failure, respectively
 * Expects:
 *      - Pnm_ppm image file or compressed image file
 *      - Command to compress (-c) or decompress (-d)
 *      - At most one file given
 * Notes:
 *      - Reads from stdin if not given a file in as an argument
 *      - Exits with 1 if given improperly formatted arguments
 *      - Raises CRE if given more than one file
 ****************************************************************************/
int main(int argc, char *argv[])
{
        int i;

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-c") == 0) {
                        compress_or_decompress = compress40;
                } else if (strcmp(argv[i], "-d") == 0) {
                        compress_or_decompress = decompress40;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n",
                                argv[0], argv[i]);
                        exit(1);
                } else if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s -d [filename]\n"
                                "       %s -c [filename]\n",
                                argv[0], argv[0]);
                        exit(1);
                } else {
                        break;
                }
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                compress_or_decompress(fp);
                fclose(fp);
        } else {
                compress_or_decompress(stdin);
        }

        return EXIT_SUCCESS; 
}