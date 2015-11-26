#include <stdlib.h>
#include "cst_alloc.h"
#include "cst_endian.h"

const int cst_endian_loc = 1;

void swap_bytes_short(short *b, int n)
{
    int i;

    for (i=0; i<n; i++)
	b[i] = SWAPSHORT(b[i]);
}

void swapdouble(double *dbl)
{
    /* cast to int, as access as flt may cause FPE on some machines */
    int t;
    int *dd = (int *)dbl;
    t = SWAPINT(dd[0]);
    dd[0] = SWAPINT(dd[1]);
    dd[1] = t;
}

void swapfloat(float *flt)
{
    /* cast to int, as access as flt may cause FPE on some machines */
    int *ff = (int *)flt;
    ff[0] = SWAPINT(ff[0]);
}

