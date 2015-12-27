#ifndef _CST_TRACK_H__
#define _CST_TRACK_H__

#include "cst_file.h"
#include "cst_error.h"
#include "cst_alloc.h"
#include "cst_endian.h"
#include "cst_file.h"
#include "cst_val.h"

typedef struct  cst_track_struct {
    const char *type;
    int num_frames;
    int num_channels;
    float *times;
    float **frames;
} cst_track;

cst_track *new_track();
void delete_track(cst_track *val);

float track_frame_shift(cst_track *t, int frame);
void cst_track_resize(cst_track *t,int num_frames, int num_channels);

int cst_track_save_est(cst_track *t, const char *filename);
int cst_track_save_est_binary(cst_track *t, const char *filename);
int cst_track_load_est(cst_track *t, const char *filename);

CST_VAL_USER_TYPE_DCLS(track,cst_track)

#endif
