#include "cst_audio.h"

cst_audiodev * audio_open_command(int sps, int channels, int fmt)
{
    cst_audiodev *ad;

    ad = cst_alloc(cst_audiodev,1);
    ad->sps = sps;
    ad->channels = channels;
    ad->fmt = fmt;
    return NULL;
}

int audio_close_command(cst_audiodev *ad)
{
    if (ad)
	cst_free(ad);
    return 0;
}

int audio_write_command(cst_audiodev *ad, void *samples, int num_bytes)
{
    (void)ad;
    (void)samples;
    return num_bytes;
}

int audio_drain_command(cst_audiodev *ad)
{
    (void)ad;
    return 0;
}

int audio_flush_command(cst_audiodev *ad)
{
    (void)ad;
    return 0;
}
