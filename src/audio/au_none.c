#include "cst_string.h"
#include "cst_wave.h"
#include "cst_audio.h"

cst_audiodev * audio_open_none(int sps, int channels, int fmt)
{
    cst_audiodev *ad;

    ad = cst_alloc(cst_audiodev,1);
    ad->sps = ad->real_sps = sps;
    ad->channels = ad->real_channels = channels;
    ad->fmt = ad->real_fmt = fmt;
    return ad;
}

int audio_close_none(cst_audiodev *ad)
{
    if (ad)
	cst_free(ad);
    return 0;
}

int audio_write_none(cst_audiodev *ad, void *samples, int num_bytes)
{
    (void)ad;
    (void)samples;
    return num_bytes;
}

int audio_drain_none(cst_audiodev *ad)
{
    (void)ad;
    return 0;
}

int audio_flush_none(cst_audiodev *ad)
{
    (void)ad;
    return 0;
}
