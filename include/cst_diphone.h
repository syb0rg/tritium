#ifndef _CST_DIPHONE_H__
#define _CST_DIPHONE_H__

#include "cst_file.h"
#include "cst_val.h"
#include "cst_features.h"
#include "cst_wave.h"
#include "cst_track.h"
#include "cst_sts.h"
#include "cst_hrg.h"

struct cst_diphone_entry_struct {
    char *name;
    unsigned short start_pm;
    unsigned char pb_pm;
    unsigned char end_pm;
};
typedef struct cst_diphone_entry_struct cst_diphone_entry;

struct cst_diphone_db_struct {
    const char *name;
    int  num_entries;
    const cst_diphone_entry *diphones;
    const cst_sts_list *sts;
};
typedef struct cst_diphone_db_struct cst_diphone_db;

CST_VAL_USER_TYPE_DCLS(diphone_db,cst_diphone_db)

cst_utterance* diphone_synth(cst_utterance *utt);
cst_utterance *get_diphone_units(cst_utterance *utt);

#endif
