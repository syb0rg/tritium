#include "cst_string.h"
#include "cst_wave.h"
#include "cst_audio.h"

CST_VAL_REGISTER_TYPE(audio_streaming_info,cst_audio_streaming_info)

cst_audio_streaming_info *new_audio_streaming_info(void)
{
    cst_audio_streaming_info *asi = 
        cst_alloc(struct cst_audio_streaming_info_struct,1);

    asi->min_buffsize = 256;
    asi->asc = NULL;
    asi->userdata = NULL;

    return asi;
}

void delete_audio_streaming_info(cst_audio_streaming_info *asi)
{
    if (asi)
        cst_free(asi);
    return;
}

int audio_stream_chunk(const cst_wave *w, int start, int size, 
                       int last, void *user)
{
    /* Called with new samples from start for size samples */
    /* last is true if this is the last segment. */
    /* This is really just and example that you can copy for you streaming */
    /* function */
    /* This particular example is *not* thread safe */
    int n;
    static cst_audiodev *ad = 0;

    if (start == 0)
        ad = audio_open(w->sample_rate,w->num_channels,CST_AUDIO_LINEAR16);

    n = audio_write(ad,&w->samples[start],size*sizeof(short));

    if (last == 1)
    {
        audio_close(ad);
        ad = NULL;
    }

    /* if you want to stop return CST_AUDIO_STREAM_STOP */
    return CST_AUDIO_STREAM_CONT;
}

