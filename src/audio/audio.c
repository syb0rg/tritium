#include "cst_string.h"
#include "cst_wave.h"
#include "cst_audio.h"
#include "native_audio.h"
#include <portaudio.h>

int audio_bps(cst_audiofmt fmt)
{
    switch (fmt)
    {
    case CST_AUDIO_LINEAR16:
	return 2;
    case CST_AUDIO_LINEAR8:
    case CST_AUDIO_MULAW:
	return 1;
    }
    return 0;
}

cst_audiodev *audio_open(int sps, int channels, cst_audiofmt fmt)
{
    cst_audiodev *ad;
    int up, down;

    ad = AUDIO_OPEN_NATIVE(sps, channels, fmt);
    if (ad == NULL)
	return NULL;

    down = sps / 1000;
    up = ad->real_sps / 1000;

    if (up != down)
	ad->rateconv = new_rateconv(up, down, channels);

    return ad;
}

int audio_close(cst_audiodev *ad)
{
    if (ad->rateconv)
	delete_rateconv(ad->rateconv);

    return AUDIO_CLOSE_NATIVE(ad);
}

int audio_write(cst_audiodev *ad,void *buff,int num_bytes)
{
    void *abuf = buff, *nbuf = NULL;
    int rv, i, real_num_bytes = num_bytes;

    if (ad->rateconv)
    {
	short *in, *out;
	int insize, outsize, n;

	insize = real_num_bytes / 2;
	in = (short *)buff;

	outsize = ad->rateconv->outsize;
	nbuf = out = cst_alloc(short, outsize);
	real_num_bytes = outsize * 2;

	while ((n = cst_rateconv_in(ad->rateconv, in, insize)) > 0)
	{
	    in += n; 
	    insize -= n;
	    while ((n = cst_rateconv_out(ad->rateconv, out, outsize)) > 0)
	    {
		out += n; 
		outsize -= n;
	    }
	}
	real_num_bytes -= outsize * 2;
	if (abuf != buff)
	    cst_free(abuf);
	abuf = nbuf;
    }
    if (ad->real_channels != ad->channels)
    {
	/* Yeah, we only do mono->stereo for now */
	if (ad->real_channels != 2 || ad->channels != 1)
	{
	    cst_errmsg("audio_write: unsupported channel mapping requested (%d => %d).\n",
		       ad->channels, ad->real_channels);
	}
	nbuf = cst_alloc(char, real_num_bytes * ad->real_channels / ad->channels);

	if (audio_bps(ad->fmt) == 2)
	{
	    for (i = 0; i < real_num_bytes / 2; ++i)
	    {
		((short *)nbuf)[i*2] = ((short *)abuf)[i];
		((short *)nbuf)[i*2+1] = ((short *)abuf)[i];
	    }
	}
	else if (audio_bps(ad->fmt) == 1)
	{
	    for (i = 0; i < real_num_bytes / 2; ++i)
	    {
		((unsigned char *)nbuf)[i*2] = ((unsigned char *)abuf)[i];
		((unsigned char *)nbuf)[i*2+1] = ((unsigned char *)abuf)[i];
	    }
	}
	else
	{
	    cst_errmsg("audio_write: unknown format %d\n", ad->fmt);
	    cst_free(nbuf);
	    if (abuf != buff)
		cst_free(abuf);
	    cst_error();
	}

	if (abuf != buff)
	    cst_free(abuf);
	abuf = nbuf;
	real_num_bytes = real_num_bytes * ad->real_channels / ad->channels;
    }
    if (ad->real_fmt != ad->fmt)
    {
	if (ad->real_fmt == CST_AUDIO_LINEAR16
	    && ad->fmt == CST_AUDIO_MULAW)
	{
	    nbuf = cst_alloc(char, real_num_bytes * 2);
	    for (i = 0; i < real_num_bytes; ++i)
		((short *)nbuf)[i] = cst_ulaw_to_short(((unsigned char *)abuf)[i]);
	    real_num_bytes *= 2;
	}
	else if (ad->real_fmt == CST_AUDIO_MULAW
		 && ad->fmt == CST_AUDIO_LINEAR16)
	{
	    nbuf = cst_alloc(char, real_num_bytes / 2);
	    for (i = 0; i < real_num_bytes / 2; ++i)
		((unsigned char *)nbuf)[i] = cst_short_to_ulaw(((short *)abuf)[i]);
	    real_num_bytes /= 2;
	}
	else if (ad->real_fmt == CST_AUDIO_LINEAR8
		 && ad->fmt == CST_AUDIO_LINEAR16)
	{
	    nbuf = cst_alloc(char, real_num_bytes / 2);
	    for (i = 0; i < real_num_bytes / 2; ++i)
		((unsigned char *)nbuf)[i] = (((short *)abuf)[i] >> 8) + 128;
	    real_num_bytes /= 2;
	}
	else
	{
	    cst_errmsg("audio_write: unknown format conversion (%d => %d) requested.\n",
		       ad->fmt, ad->real_fmt);
	    cst_free(nbuf);
	    if (abuf != buff)
		cst_free(abuf);
	    cst_error();
	}
	if (abuf != buff)
	    cst_free(abuf);
	abuf = nbuf;
    }
    if (ad->byteswap && audio_bps(ad->real_fmt) == 2)
	swap_bytes_short((short *)abuf, real_num_bytes/2);

    if (real_num_bytes)
	rv = AUDIO_WRITE_NATIVE(ad,abuf,real_num_bytes);
    else
	rv = 0;

    if (abuf != buff)
	cst_free(abuf);

    /* Callers expect to get the same num_bytes back as they passed
       in.  Funny, that ... */
    return (rv == real_num_bytes) ? num_bytes : 0;
}

int audio_drain(cst_audiodev *ad)
{
    return AUDIO_DRAIN_NATIVE(ad);
}

int audio_flush(cst_audiodev *ad)
{
    return AUDIO_FLUSH_NATIVE(ad);
}

static int playCallback( const void *inputBuffer, void *outputBuffer,
						unsigned long framesPerBuffer,
						const PaStreamCallbackTimeInfo* timeInfo,
						PaStreamCallbackFlags statusFlags,
						void *userData )
{
    cst_wave *data = (cst_wave*)userData;
    short *rptr = &data->samples[data->frameIndex * data->num_channels];
    short *wptr = (short*)outputBuffer;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        /* final buffer... */
        memcpy(wptr, rptr, sizeof(*wptr) * data->num_channels * framesLeft);
        memset(wptr, sizeof(*wptr) * data->num_channels * framesPerBuffer, 0);
        data->frameIndex += framesLeft;
        finished = paComplete;
    }
    else
    {
        memcpy(wptr, rptr, sizeof(*wptr) * data->num_channels * framesPerBuffer);
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

int play_wave(cst_wave *w)
{
	PaStream* stream;
	PaStreamParameters outputParameters;
	cst_wave_set_frameIndex(w, 0);
	cst_wave_set_maxFrameIndex(w, cst_wave_num_samples(w) * cst_wave_num_channels(w));
	int err = 0;
	err = Pa_Initialize();
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice)
	{
		fprintf(stderr,"Error: No default output device.\n");
		return -5;
	}

	outputParameters.channelCount = cst_wave_num_channels(w);
	outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream(&stream,
						NULL, /* no input */
						&outputParameters,
						cst_wave_sample_rate(w),
						sizeof(short) * cst_wave_num_channels(w),
						paClipOff,
						playCallback,
						w);
	if( stream )
    {
        err = Pa_StartStream( stream );
        if( err != paNoError ) goto done;


        while((err = Pa_IsStreamActive(stream)) == 1) Pa_Sleep(100);
        if( err < 0 ) goto done;

        err = Pa_CloseStream( stream );
        if( err != paNoError ) goto done;
	}
done:
    Pa_Terminate();
	return 0;
}

int play_wave_sync(cst_wave *w, cst_relation *rel,
		   int (*call_back)(cst_item *))
{
    int q,i,n,r;
    cst_audiodev *ad;
    float r_pos;
    cst_item *item;

    if (!w)
	return CST_ERROR_FORMAT;
    
    if ((ad = audio_open(w->sample_rate,w->num_channels,
			 CST_AUDIO_LINEAR16)) == NULL)
	return CST_ERROR_FORMAT;

    q=0;
    item = relation_head(rel);
    r_pos = w->sample_rate * 0;
    for (i=0; i < w->num_samples; i += r/2)
    {
	if (i >= r_pos)
	{
	    audio_flush(ad);

	    if ((*call_back)(item) != CST_OK_FORMAT)
		break;
	    item = item_next(item);
	    if (item)
		r_pos = w->sample_rate * val_float(ffeature(item,"p.end"));
	    else
		r_pos = w->num_samples;
	}
	if (w->num_samples > i+CST_AUDIOBUFFSIZE)
	    n = CST_AUDIOBUFFSIZE;
	else
	    n = w->num_samples-i;
	r = audio_write(ad,&w->samples[i],n*2);
	q +=r;
	if (r <= 0)
	    cst_errmsg("failed to write %d samples\n",n);
    }

    audio_close(ad);

    return CST_OK_FORMAT;
}
