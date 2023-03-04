#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"
#include "a2blocked.h"

A2Methods_UArray2 int_to_float(A2Methods_UArray2 image);
A2Methods_UArray2 float_to_int(A2Methods_UArray2 a2_f);
void free_elements(int col, int row, A2Methods_UArray2 a2, void *elem, 
                   void *cl);
// void convert_to_float(int col, int row, A2Methods_UArray2 a2, 
//                       void *elem, void *cl);