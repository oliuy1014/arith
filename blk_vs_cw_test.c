#include <stdio.h>
#include <stdlib.h>
#include "blk_vs_cw.h"

int main() 
{
        fcoords a = malloc(sizeof(*a));
        fcoords b = malloc(sizeof(*b));
        fcoords c = malloc(sizeof(*c));
        fcoords d = malloc(sizeof(*d));
        fcoords pb = malloc(sizeof(*pb));
        fcoords pr = malloc(sizeof(*pr));
        fcoords cw_format[] = {a, b, c, d, pb, pr};
        SET_FORMAT(cw_format);
        for (int i = 0; i < 6; i++) {
                printf("width: %d, lsb: %d\n",
                (cw_format[i])->width, (cw_format[i]->lsb));
        }
}