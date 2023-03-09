#include <stdio.h>
#include "a2plain.h"
#include "pnm.h"

A2Methods_UArray2 image_to_a2(FILE *image);
void a2_to_image(A2Methods_UArray2 a2_i);
A2Methods_UArray2 trim(A2Methods_UArray2 a2, int width, int height);