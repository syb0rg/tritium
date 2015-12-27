#ifndef _CST_STS_H__
#define _CST_STS_H__

/* Need some lower level functions in case we are doing streaming */
#include "cst_wave.h"
#include "cst_audio.h"

/* The short term signal (sts) structure is the basic unit data info  */
/* it may be diphones or general units.  Indexes and names are held   */
/* else where, this information plus the indexes in the Unit relation */
/* allow reconstruction of the signal itself                          */
struct cst_sts_struct {
    const unsigned short *frame;  
    const int size;      /* in samples */
    const unsigned char *residual;
};
typedef struct cst_sts_struct cst_sts;

/* Because many C compilers can't compile when there are 100Ks of symbols */
/* We store the sts in pages.  Each page of stss gets compiled into an */
/* array in separate files thus reducing the number of symbols -- but */
/* introducing an extra dereference */
struct cst_sts_paged_struct {
    const unsigned short frame_offset;
    const unsigned short res_size;
    const unsigned int res_offset;
    const unsigned short *frame_page;  
    const unsigned char *res_page;  
};
typedef struct cst_sts_paged_struct cst_sts_paged;

/* This represents a database of short-term signals. */
struct cst_sts_list_struct {
    /* If the sts are compiled in, this will point to them. */
    const cst_sts *sts;
    /* Or if the data is paged in different structures */
    const cst_sts_paged *sts_paged;
    /* Or we could have these set (or set later) */
    const unsigned short *frames;
    const unsigned char *residuals;
    const unsigned int *resoffs;

    int num_sts;          /* But I don't think you need that number */
    int num_channels;     /* typically lpc order */
    int sample_rate;
    float coeff_min;      /* used for decoding the short representation */
    float coeff_range;    /* for coefficients  */ 
};
typedef struct cst_sts_list_struct cst_sts_list;

/* This is used to represent a newly constructed waveform to be synthed */
struct cst_lpcres_struct {
    const unsigned short **frames;
    int *times;
    int num_frames;
    int num_channels;
    float lpc_min;
    float lpc_range;
    int num_samples;
    int sample_rate;
    int *sizes;
    unsigned char *residual;

    /* Optional call back function */
    cst_audio_streaming_info *asi;
};
typedef struct cst_lpcres_struct cst_lpcres;

cst_lpcres *new_lpcres();
void delete_lpcres(cst_lpcres *l);
float lpcres_frame_shift(cst_lpcres *t, int frame);
void lpcres_resize_frames(cst_lpcres *l,int num_frames);
void lpcres_resize_samples(cst_lpcres *l,int num_samples);

cst_sts_list *new_sts_list();
void delete_sts_list(cst_sts_list *l);

const unsigned short * get_sts_frame(const cst_sts_list *sts_list, int frame);
const unsigned char * get_sts_residual(const cst_sts_list *sts_list, int frame);
const unsigned char * get_sts_residual_fixed(const cst_sts_list *sts_list, int frame);

int get_frame_size(const cst_sts_list *sts_list, int frame);
int get_unit_size(const cst_sts_list *s,int start, int end);

CST_VAL_USER_TYPE_DCLS(lpcres,cst_lpcres)
CST_VAL_USER_TYPE_DCLS(sts_list,cst_sts_list)

#endif
