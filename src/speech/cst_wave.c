#include "cst_string.h"
#include "cst_val.h"
#include "cst_wave.h"

CST_VAL_REGISTER_TYPE(wave,cst_wave)

cst_wave *new_wave()
{
    cst_wave *w = cst_alloc(struct cst_wave_struct,1);
    w->type = NULL;
    w->num_samples = 0;
    w->samples = NULL;
    return w;
}

void delete_wave(cst_wave *w)
{
    if (w)
    {
	cst_free(w->samples);
	cst_free(w);
    }
    return;
}

void cst_wave_resize(cst_wave *w,int samples, int num_channels)
{
    short *ns;

    if (!w)
    {
	cst_errmsg("cst_wave_resize: null wave given to resize\n");
	cst_error();
    }
    ns = cst_alloc(short,samples*num_channels);
    if (num_channels == w->num_channels)
	memmove(ns,w->samples,
		sizeof(short) * 
		num_channels *
		(samples < w->num_samples ? samples : w->num_samples));
    cst_free(w->samples);
    w->samples = ns;
    w->num_samples = samples;
    w->num_channels = num_channels;

}

void cst_wave_rescale(cst_wave *w, int factor)
{
	int i;

	for (i = 0; i < w->num_samples; ++i)
		w->samples[i] = ((int)w->samples[i] * factor) / 65536;
}

cst_wave *copy_wave(const cst_wave *w)
{
    cst_wave *n = new_wave();

    cst_wave_resize(n,w->num_samples,w->num_channels);
    n->sample_rate = w->sample_rate;
    n->num_channels = w->num_channels;
    n->type = w->type;
    memcpy(n->samples,w->samples,sizeof(short)*w->num_samples*w->num_channels);
    return n;
}

cst_wave *concat_wave(cst_wave *dest, const cst_wave *src)
{
    int orig_nsamps;

    if (dest->num_channels != src->num_channels)
    {
	cst_errmsg("concat_wave: channel count mismatch (%d != %d)\n",
		   dest->num_channels, src->num_channels);
	cst_error();
    }
    if (dest->sample_rate != src->sample_rate)
    {
	cst_errmsg("concat_wave: sample rate mismatch (%d != %d)\n",
		   dest->sample_rate, src->sample_rate);
	cst_error();
    }

    orig_nsamps = dest->num_samples * dest->num_channels;
    cst_wave_resize(dest, dest->num_samples + src->num_samples,
		    dest->num_channels);
    memcpy(dest->samples + orig_nsamps, src->samples,
	   src->num_samples * src->num_channels * sizeof(short));

    return dest;
}
