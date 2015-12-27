#ifndef _CST_LTS_H__
#define _CST_LTS_H__

#include "cst_val.h"

typedef unsigned short cst_lts_addr;
typedef int cst_lts_phone;
typedef unsigned char  cst_lts_feat;
typedef unsigned char  cst_lts_letter;
typedef unsigned char  cst_lts_model;

/* end of rule value */
#define CST_LTS_EOR 255

typedef struct cst_lts_rules_struct {
    char *name;
    const cst_lts_addr *letter_index;  /* index into model first state */
    const cst_lts_model *models;
    const char * const * phone_table;
    int context_window_size;
    int context_extra_feats;
    const char * const * letter_table;
} cst_lts_rules;

/* Note this is designed to be 6 bytes */
typedef struct cst_lts_rule_struct {
    cst_lts_feat   feat;
    cst_lts_letter val;
    cst_lts_addr   qtrue;
    cst_lts_addr   qfalse;
} cst_lts_rule;

cst_lts_rules *new_lts_rules();

cst_val *lts_apply(const char *word,const char *feats,const cst_lts_rules *r);
cst_val *lts_apply_val(const cst_val *wlist,const char *feats,const cst_lts_rules *r);

#endif

