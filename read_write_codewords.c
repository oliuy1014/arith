/*****************************************************************************
 *
 *                 read_write_codewords.c
 *
 *      Assignment: Homework 4 -- arith
 *      Authors: Rivan Mehra (rmehra2), Oliver Uy (ouy01)
 *      Date: March 9th, 2023
 *     
 *      Summary: 
 *      - Compression: Writes compressed image to a file from a UArray2
 *      - Decompression: Reads codewords from a compressed image file and
                         adds codewords to a UArray2 
 *
 *      
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "structs_and_constants.h"
#include "bitpack.h"
#include <assert.h>
#define WORD_BITS 32
#define WORD_BYTES 4
#define BITS_IN_BYTE 8

void write_codeword_apply(int col, int row, A2Methods_UArray2 a2_cw,
                          void *elem, void *cl);

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
void write_codewords(A2Methods_UArray2 a2_cw)
{
        assert(a2_cw != NULL);
        int width = 2 * (METHODS_P->width(a2_cw));
        int height = 2 * (METHODS_P->height(a2_cw));
        printf("COMP40 Compressed image format 2\n%u %u\n", width, height);
        METHODS_P->map_default(a2_cw, write_codeword_apply, NULL);
        METHODS_P->free(&a2_cw);
}

/**********write_codeword_apply***********************************************
 *
 * Purpose:
 *      Writes a single codeword at col, row in array to file.
 * Parameters: 
 *      int col: column where the codeword is in a2_cw
 *      int row: row where the codeword is in a2_c
 *      A2Methods_UArray2 a2_cw: UArray2 from which the codewords are read
 *      void *elem: a pointer to the element at col, row in a2_cw
 *      void *cl: closure pointer, NULL for this function
 * Returns: 
 *      none
 * Expects:
 *      elem to not be NULL
 * Notes:
 *      Puts codewords to file in big endian order.
 *      Raises CRE if elem is null.
 ****************************************************************************/
void write_codeword_apply(int col, int row, A2Methods_UArray2 a2_cw,
                      void *elem, void *cl)
{
        (void) col;
        (void) row;
        (void) a2_cw;
        (void) cl;
        unsigned curr_byte;
        assert(elem != NULL);
        uint32_t cw = *(uint32_t *) elem;
        for (int i = 0; i < WORD_BYTES; i++) {
                curr_byte = Bitpack_getu(cw, BITS_IN_BYTE, WORD_BITS - 
                                         BITS_IN_BYTE * (i + 1));
                putchar(curr_byte);
        }
}

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
A2Methods_UArray2 read_codewords(FILE* compressed_file) 
{
        assert(compressed_file != NULL);
        unsigned height, width;
        int read = fscanf(compressed_file, 
                          "COMP40 Compressed image format 2\n%u %u",
                          &width, &height);
        assert(read == 2);
        int c = getc(compressed_file);
        assert(c == '\n');
        assert(width >= MIN_WIDTH && width % 2 == 0);
        assert(height >= MIN_HEIGHT && height % 2 == 0);
        width = width / 2;
        height = height / 2;
        A2Methods_UArray2 a2_cw = METHODS_P->new(width, height,
                                                 sizeof(uint32_t));
        uint32_t cw = 0;
        unsigned idx = 0;
        unsigned byte_1 = fgetc(compressed_file);
        unsigned byte_2 = fgetc(compressed_file);
        unsigned byte_3 = fgetc(compressed_file);
        unsigned byte_4 = fgetc(compressed_file);
        while (!(feof(compressed_file)) && idx < (width * height)) {
                unsigned bytes[4] = {byte_1, byte_2, byte_3, byte_4};
                for (int i = 0; i < WORD_BYTES; i++) {
                        cw = Bitpack_newu(cw, BITS_IN_BYTE,
                                        WORD_BITS - (BITS_IN_BYTE * (i + 1)),
                                        bytes[i]);
                        *(uint32_t*) METHODS_P->at(a2_cw, 
                                                   (idx % width),
                                                   (idx / width)) 
                                                   = cw;
                }
                byte_1 = fgetc(compressed_file);
                byte_2 = fgetc(compressed_file);
                byte_3 = fgetc(compressed_file);
                byte_4 = fgetc(compressed_file);
                idx++;
        }

        /* check that file was long enough for stated width and height */
        assert(idx == width * height);
        return a2_cw;
}