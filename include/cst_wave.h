#ifndef _CST_WAVE_H__
#define _CST_WAVE_H__

#include "cst_file.h"
#include "cst_error.h"
#include "cst_alloc.h"
#include "cst_endian.h"
#include "cst_file.h"
#include "cst_val.h"

typedef struct  cst_wave_struct {
    const char *type;
	unsigned int frameIndex;
	int maxFrameIndex;
    int sample_rate;
    int num_samples;
    int num_channels;
    short *samples;
} cst_wave;

typedef struct  cst_wave_header_struct {
    const char *type;
    int hsize;
    int num_bytes;
    int sample_rate;
    int num_samples;
    int num_channels;
} cst_wave_header;

cst_wave *new_wave();
cst_wave *copy_wave(const cst_wave *w);
void delete_wave(cst_wave *val);
cst_wave *concat_wave(cst_wave *dest, const cst_wave *src);

#define cst_wave_num_samples(w) (w?w->num_samples:0)
#define cst_wave_num_channels(w) (w?w->num_channels:0)
#define cst_wave_sample_rate(w) (w?w->sample_rate:0)
#define cst_wave_samples(w) (w->samples)
#define cst_wave_frameIndex(w) (w->frameIndex)
#define cst_wave_maxFrameIndex(w) (w->maxFrameIndex)

#define cst_wave_set_num_samples(w,s) w->num_samples=s
#define cst_wave_set_num_channels(w,s) w->num_channels=s
#define cst_wave_set_sample_rate(w,s) w->sample_rate=s
#define cst_wave_set_frameIndex(w,s) w->frameIndex=s
#define cst_wave_set_maxFrameIndex(w,s) w->maxFrameIndex=s

int cst_wave_save(cst_wave *w, const char *filename, const char *type);
int cst_wave_save_riff(cst_wave *w, const char *filename);
int cst_wave_save_raw(cst_wave *w, const char *filename);
int cst_wave_append_riff(cst_wave *w,const char *filename);

int cst_wave_save_riff_fd(cst_wave *w, cst_file fd);
int cst_wave_save_raw_fd(cst_wave *w, cst_file fd);

int cst_wave_load(cst_wave *w, const char *filename, const char *type);
int cst_wave_load_riff(cst_wave *w, const char *filename);
int cst_wave_load_raw(cst_wave *w, const char *filename,
				const char *bo, int sample_rate);

int cst_wave_load_riff_header(cst_wave_header *header,cst_file fd);
int cst_wave_load_riff_fd(cst_wave *w, cst_file fd);
int cst_wave_load_raw_fd (cst_wave *w, cst_file fd,
				    const char *bo, int sample_rate);

void cst_wave_resize(cst_wave *w,int samples, int num_channels);
void cst_wave_resample(cst_wave *w, int sample_rate);
void cst_wave_rescale(cst_wave *w, int factor);

/* Resampling code */
typedef struct cst_rateconv_struct {
	int channels;           /* what do you think? */
	int up, down;           /* up/down sampling ratio */

	double gain;            /* output gain */
	int lag;                /* lag time (in samples) */
	int *sin, *sout, *coep; /* filter buffers, coefficients */

	/* n.b. outsize is the minimum buffer size for
           cst_rateconv_out() when streaming */
	int insize, outsize;    /* size of filter buffers */
	int incount;		/* amount of input data */
	int len;		/* size of filter */

	/* internal foo coefficients */
	double fsin, fgk, fgg;
	/* internal counters */
	int inbaseidx, inoffset, cycctr, outidx;
} cst_rateconv;

cst_rateconv * new_rateconv(int up, int down, int channels);
void delete_rateconv(cst_rateconv *filt);
int cst_rateconv_in(cst_rateconv *filt, const short *inptr, int max);
int cst_rateconv_leadout(cst_rateconv *filt);
int cst_rateconv_out(cst_rateconv *filt, short *outptr, int max);

/* File format cruft. */

#define RIFF_FORMAT_PCM    0x0001
#define RIFF_FORMAT_ADPCM  0x0002
#define RIFF_FORMAT_MULAW  0x0006
#define RIFF_FORMAT_ALAW   0x0007

/* Sun/Next header, short and sweet, note its always BIG_ENDIAN though */
typedef struct {
    unsigned int    magic;	/* magic number */
    unsigned int    hdr_size;	/* size of this header */
    int             data_size;	/* length of data (optional) */
    unsigned int    encoding;	/* data encoding format */
    unsigned int    sample_rate; /* samples per second */
    unsigned int    channels;	 /* number of interleaved channels */
} snd_header;

#define CST_SND_MAGIC (unsigned int)0x2e736e64
#define CST_SND_ULAW  1
#define CST_SND_UCHAR 2
#define CST_SND_SHORT 3

/* Convertion functions */
unsigned char cst_short_to_ulaw(short sample);
short cst_ulaw_to_short(unsigned char ulawbyte);

CST_VAL_USER_TYPE_DCLS(wave,cst_wave)

#endif
