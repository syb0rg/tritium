#include "cst_string.h"
#include "cst_val.h"
#include "cst_track.h"

CST_VAL_REGISTER_TYPE(track,cst_track)

cst_track *new_track()
{
    cst_track *w = cst_alloc(struct cst_track_struct,1);
    return w;
}

void delete_track(cst_track *w)
{
    int i;

    if (w)
    {
	cst_free(w->times);
	for (i=0; i < w->num_frames; i++)
	    cst_free(w->frames[i]);
	cst_free(w->frames);
	cst_free(w);
    }
    return;
}

float track_frame_shift(cst_track *t, int frame)
{
    if (frame == 0)
	return t->times[frame];
    else
	return t->times[frame]-t->times[frame-1];
}

void cst_track_resize(cst_track *t,int num_frames, int num_channels)
{
    float *n_times;
    float **n_frames;
    int i;

    n_times = cst_alloc(float,num_frames);
    memmove(n_times,t->times,
	    (sizeof(float)*((num_frames < t->num_frames) ? 
			    num_frames : t->num_frames)));
    n_frames = cst_alloc(float*,num_frames);
    for (i=0; i<num_frames; i++)
    {
	n_frames[i] = cst_alloc(float,num_channels);
	if (i<t->num_frames)
	{
	    memmove(n_frames[i],
		    t->frames[i],
		    sizeof(float)*((num_channels < t->num_channels) ?
				   num_channels : t->num_channels));
	    cst_free(t->frames[i]);
	}
    }
    for (   ; i<t->num_frames; i++)
	cst_free(t->frames[i]);
    cst_free(t->frames);
    t->frames = n_frames;
    cst_free(t->times);
    t->times = n_times;
    t->num_frames = num_frames;
    t->num_channels = num_channels;
}



