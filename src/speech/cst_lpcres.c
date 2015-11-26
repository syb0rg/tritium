#include "cst_string.h"
#include "cst_val.h"
#include "cst_sts.h"

CST_VAL_REGISTER_TYPE(lpcres,cst_lpcres)

cst_lpcres *new_lpcres()
{
    cst_lpcres *l = cst_alloc(struct cst_lpcres_struct,1);
    return l;
}

void delete_lpcres(cst_lpcres *l)
{
    if (l)
    {
	cst_free(l->times);
	cst_free((unsigned short **)l->frames);
	cst_free(l->residual);
	cst_free(l->sizes);
	cst_free(l);
    }
    return;
}

float lpcres_frame_shift(cst_lpcres *t, int frame)
{
    if (frame == 0)
	return (float) t->times[frame];
    else
	return (float) t->times[frame]-t->times[frame-1];
}

void lpcres_resize_frames(cst_lpcres *l,int num_frames)
{
    l->times = cst_alloc(int,num_frames);
    l->frames = cst_alloc(const unsigned short*,num_frames);
    l->sizes = cst_alloc(int,num_frames);
    l->num_frames = num_frames;
}

void lpcres_resize_samples(cst_lpcres *l,int num_samples)
{
    l->residual = cst_alloc(unsigned char,num_samples);
    /* mulaw for 0 is 255 */
    memset(l->residual,255,num_samples);
    l->num_samples = num_samples;

}
