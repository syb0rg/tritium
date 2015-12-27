#ifndef _CST_AUDIO_H__
#define _CST_AUDIO_H__

#include "cst_wave.h"
#include "cst_hrg.h"

#ifdef CST_AUDIO_WIN32
#define CST_AUDIOBUFFSIZE 8092
#else
#define CST_AUDIOBUFFSIZE 128
#endif

#define CST_AUDIO_DEFAULT_PORT 1746
#define CST_AUDIO_DEFAULT_SERVER "localhost"
#define CST_AUDIO_DEFAULT_ENCODING "short"

typedef enum {
    CST_AUDIO_LINEAR16 = 0,
    CST_AUDIO_LINEAR8,
    CST_AUDIO_MULAW
} cst_audiofmt;
/* Returns the number of bytes per sample for a given audio format */
int audio_bps(cst_audiofmt fmt);

typedef struct cst_audiodev_struct {
    int sps, real_sps;
    int channels, real_channels;
    cst_audiofmt fmt, real_fmt;
    int byteswap;
    cst_rateconv *rateconv;
    void *platform_data;
} cst_audiodev;

/* Generic audio functions */
cst_audiodev *audio_open(int sps, int channels, cst_audiofmt fmt);
int audio_close(cst_audiodev *ad);
int audio_write(cst_audiodev *ad, void *buff, int num_bytes);
int audio_flush(cst_audiodev *ad); /* wait for buffers to empty */
int audio_drain(cst_audiodev *ad); /* empty buffers now */

/* Generic high level audio functions */
int play_wave(cst_wave *w);
int play_wave_sync(cst_wave *w, cst_relation *rel,
                   int (*call_back)(cst_item *));
int play_wave_client(cst_wave *w, const char *servername, int port,
                     const char *encoding);
int auserver(int port);

/* Play wave to specified device */
int play_wave_device(cst_wave *w, cst_audiodev *ad);

/* Output to a file as if its an audio device */
cst_audiodev *audio_open_file(int sps, int channels, cst_audiofmt fmt,
			      const char *filename);
int audio_close_file(cst_audiodev *ad);
int audio_write_file(cst_audiodev *ad, void *buff, int num_bytes);
int audio_drain_file(cst_audiodev *ad);
int audio_flush_file(cst_audiodev *ad);

/* For audio streaming */
#define CST_AUDIO_STREAM_STOP -1
#define CST_AUDIO_STREAM_CONT 0
typedef int (*cst_audio_stream_callback)(const cst_wave *w,int start,int size, 
                                      int last, void *user);
typedef struct cst_audio_streaming_info_struct
{
    int min_buffsize;
    cst_audio_stream_callback asc;
    void *userdata;
} cst_audio_streaming_info;
cst_audio_streaming_info *new_audio_streaming_info();
void delete_audio_streaming_info(cst_audio_streaming_info *asi);
CST_VAL_USER_TYPE_DCLS(audio_streaming_info,cst_audio_streaming_info)

/* An example audio streaming callback function src/audio/au_streaming.c */
int audio_stream_chunk(const cst_wave *w, int start, int size, 
                       int last, void *user);

#endif
