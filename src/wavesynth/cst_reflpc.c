#include "cst_alloc.h"

void lpc2ref(const float *lpc, float *rfc, int order)
{
    /* LPC to reflection coefficients, converted from Festival */
    int i,j;
    float f,ai;
    float *vo,*vx;
    float *vn = cst_alloc(float,order);

    i = order-1;
    rfc[i] = ai = lpc[i];
    f = 1-ai*ai;
    i--;

    for (j=0; j<=i; j++)
        rfc[j] = (lpc[j]+((ai*lpc[i-j])))/f;

    vo=rfc;

    for ( ;i>0; )
    {
        ai=vo[i];
        f = 1-ai*ai;
        i--;
	for (j=0; j<=i; j++)
            vn[j] = (vo[j]+((ai*vo[i-j])))/f;

        rfc[i]=vn[i];

        vx = vn;
        vn = vo;
        vo = vx;
    }

    cst_free(vn);

}

void ref2lpc(const float *rfc, float *lpc, int order)
{
    // Here we use Christopher Longet Higgin's algorithm converted to
    // an equivalent by awb.  Its doesn't have the reverse order or
    // negation requirement.
    float a,b;
    int n,k;

    for (n=0; n < order; n++)
    {
        lpc[n] = rfc[n];
        for (k=0; 2*(k+1) <= n+1; k++)
        {
            a = lpc[k];
            b = lpc[n-(k+1)];
            lpc[k] = a-b*lpc[n];
            lpc[n-(k+1)] = b-a*lpc[n];
        }
    }

}

