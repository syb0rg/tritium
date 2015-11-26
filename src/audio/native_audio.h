#ifndef _NATIVE_AUDIO_H__
#define _NATIVE_AUDIO_H__

#ifdef CST_AUDIO_COMMAND

#define AUDIO_OPEN_NATIVE audio_open_command
#define AUDIO_CLOSE_NATIVE audio_close_command
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_command
#define AUDIO_WRITE_NATIVE audio_write_command
#define AUDIO_DRAIN_NATIVE audio_drain_command
#define AUDIO_FLUSH_NATIVE audio_flush_command

#endif

#ifdef CST_AUDIO_SUNOS

#define AUDIO_OPEN_NATIVE audio_open_sun
#define AUDIO_CLOSE_NATIVE audio_close_sun
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_sun
#define AUDIO_WRITE_NATIVE audio_write_sun
#define AUDIO_DRAIN_NATIVE audio_drain_sun
#define AUDIO_FLUSH_NATIVE audio_flush_sun

#endif

#ifdef CST_AUDIO_LINUX

#define AUDIO_OPEN_NATIVE audio_open_oss
#define AUDIO_CLOSE_NATIVE audio_close_oss
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_oss
#define AUDIO_WRITE_NATIVE audio_write_oss
#define AUDIO_DRAIN_NATIVE audio_drain_oss
#define AUDIO_FLUSH_NATIVE audio_flush_oss

#endif

#ifdef CST_AUDIO_ALSA

#define AUDIO_OPEN_NATIVE audio_open_alsa
#define AUDIO_CLOSE_NATIVE audio_close_alsa
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_alsa
#define AUDIO_WRITE_NATIVE audio_write_alsa
#define AUDIO_DRAIN_NATIVE audio_drain_alsa
#define AUDIO_FLUSH_NATIVE audio_flush_alsa

#endif

#ifdef CST_AUDIO_FREEBSD

#define AUDIO_OPEN_NATIVE audio_open_oss
#define AUDIO_CLOSE_NATIVE audio_close_oss
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_oss
#define AUDIO_WRITE_NATIVE audio_write_oss
#define AUDIO_DRAIN_NATIVE audio_drain_oss
#define AUDIO_FLUSH_NATIVE audio_flush_oss

#endif

#ifdef CST_AUDIO_WINCE

#define AUDIO_OPEN_NATIVE audio_open_wince
#define AUDIO_CLOSE_NATIVE audio_close_wince
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_wince
#define AUDIO_WRITE_NATIVE audio_write_wince
#define AUDIO_DRAIN_NATIVE audio_drain_wince
#define AUDIO_FLUSH_NATIVE audio_flush_wince

#endif

#ifdef CST_AUDIO_WIN32
#define AUDIO_OPEN_NATIVE audio_open_wince
#define AUDIO_CLOSE_NATIVE audio_close_wince
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_wince
#define AUDIO_WRITE_NATIVE audio_write_wince
#define AUDIO_DRAIN_NATIVE audio_drain_wince
#define AUDIO_FLUSH_NATIVE audio_flush_wince

#endif

#ifdef CST_AUDIO_PALMOS

#define AUDIO_OPEN_NATIVE audio_open_palmos
#define AUDIO_CLOSE_NATIVE audio_close_palmos
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_palmos
#define AUDIO_WRITE_NATIVE audio_write_palmos
#define AUDIO_DRAIN_NATIVE audio_drain_palmos
#define AUDIO_FLUSH_NATIVE audio_flush_palmos

#endif

#ifdef CST_AUDIO_NONE

#define AUDIO_OPEN_NATIVE audio_open_none
#define AUDIO_CLOSE_NATIVE audio_close_none
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_none
#define AUDIO_WRITE_NATIVE audio_write_none
#define AUDIO_DRAIN_NATIVE audio_drain_none
#define AUDIO_FLUSH_NATIVE audio_flush_none

#endif

#ifndef AUDIO_OPEN_NATIVE

#define AUDIO_OPEN_NATIVE audio_open_none
#define AUDIO_CLOSE_NATIVE audio_close_none
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_none
#define AUDIO_WRITE_NATIVE audio_write_none
#define AUDIO_DRAIN_NATIVE audio_drain_none
#define AUDIO_FLUSH_NATIVE audio_flush_none
#define CST_AUDIO_NONE

#endif

cst_audiodev *AUDIO_OPEN_NATIVE(int sps, int channels, cst_audiofmt fmt);
int AUDIO_CLOSE_NATIVE(cst_audiodev *ad);
int AUDIO_WRITE_NATIVE(cst_audiodev *ad,void *buff,int num_bytes);
int AUDIO_DRAIN_NATIVE(cst_audiodev *ad);
int AUDIO_FLUSH_NATIVE(cst_audiodev *ad);

#endif
