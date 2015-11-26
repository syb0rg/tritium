#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "cst_string.h"
#include "cst_wave.h"
#include "cst_audio.h"

#ifdef CST_AUDIO_LINUX
/* Linux/voxware audio specific */
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <sys/types.h>
#endif
#ifdef CST_AUDIO_FREEBSD
/* probably Net and Open too */
#include <machine/soundcard.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

static const char * const oss_audio_device = "/dev/dsp";

cst_audiodev *audio_open_oss(int sps, int channels, cst_audiofmt fmt)
{
    cst_audiodev *ad;
    int afmt;
    int sfmts;
    int afd;
    int frag;

    afd = open(oss_audio_device,O_WRONLY);
    if (afd == -1)
    {
	cst_errmsg("oss_audio: failed to open audio device %s\n",
		   oss_audio_device);
	return NULL;
    }

    ad = cst_alloc(cst_audiodev, 1);
    ad->sps = sps;
    ad->channels = channels;
    ad->fmt = fmt;
    ad->platform_data = (void *)afd;

    if (ad->channels == 0)
	ad->channels = 1;

    ioctl(afd,SNDCTL_DSP_RESET,0);

    ad->real_sps = ad->sps;

    ad->real_channels = ad->channels;
    ioctl(afd,SNDCTL_DSP_CHANNELS,&ad->real_channels);
    if (ad->real_channels != ad->channels)
    {
	/* Its a stereo only device so it'll play things twice as fast */
	ad->real_sps /= 2;
    }

    ioctl(afd,SNDCTL_DSP_SPEED,&ad->real_sps);

    frag = (4 << 16) | 10;
    ioctl(afd,SNDCTL_DSP_SETFRAGMENT,&frag);

    ioctl(afd,SNDCTL_DSP_GETFMTS,&sfmts);
    if (fmt == CST_AUDIO_LINEAR8 && (sfmts & AFMT_U8))
    {
	ad->real_fmt = CST_AUDIO_LINEAR8;
	afmt = AFMT_U8;
    }
    else if (fmt == CST_AUDIO_MULAW && (sfmts & AFMT_MU_LAW))
    {
	ad->real_fmt = CST_AUDIO_MULAW;
	afmt = AFMT_MU_LAW;
    }
    else if (CST_LITTLE_ENDIAN)
    {
	if (sfmts & AFMT_S16_LE)
	{
	    ad->real_fmt = CST_AUDIO_LINEAR16;
	    afmt = AFMT_S16_LE;
	}
	else if (sfmts & AFMT_S16_BE) /* not likely */
	{
	    ad->byteswap = 1;
	    ad->real_fmt = CST_AUDIO_LINEAR16;
	    afmt = AFMT_S16_BE;
	}
	else if (sfmts & AFMT_U8)
	{
	    afmt = AFMT_U8;
	    ad->real_fmt = CST_AUDIO_LINEAR8;
	}
	else
	{
	    cst_free(ad);
	    close(afd);
	    return NULL;
	}
    }
    else
    {
	if (sfmts & AFMT_S16_BE)
	{
	    ad->real_fmt = CST_AUDIO_LINEAR16;
	    afmt = AFMT_S16_BE;
	}
	else if (sfmts & AFMT_S16_LE) /* likely */
	{
	    ad->real_fmt = CST_AUDIO_LINEAR16;
	    ad->byteswap = 1;
	    afmt = AFMT_S16_LE;
	}
	else if (sfmts & AFMT_U8)
	{
	    ad->real_fmt = CST_AUDIO_LINEAR8;
	    afmt = AFMT_U8;
	}
	else
	{
	    cst_free(ad);
	    close(afd);
	    return NULL;
	}
    }
    ioctl(afd, SNDCTL_DSP_SETFMT, &afmt);

    return ad;
}

int audio_close_oss(cst_audiodev *ad)
{
    int rv;

    if (ad == NULL)
	return 0;

    ioctl((int)ad->platform_data, SNDCTL_DSP_SYNC, NULL);
    rv = close((int)ad->platform_data);
    cst_free(ad);
    return rv;
}

int audio_write_oss(cst_audiodev *ad, void *samples, int num_bytes)
{
    return write((int)ad->platform_data,samples,num_bytes);
}

int audio_flush_oss(cst_audiodev *ad)
{
    return ioctl((int)ad->platform_data, SNDCTL_DSP_SYNC, NULL);
}

int audio_drain_oss(cst_audiodev *ad)
{
    return ioctl((int)ad->platform_data, SNDCTL_DSP_RESET, NULL);
}
