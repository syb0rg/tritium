#ifndef _CST_LTS_REWRITES_H__
#define _CST_LTS_REWRITES_H__

#include "cst_val.h"

typedef struct cst_lts_rewrites_struct {
    char *name;
    const cst_val *sets;
    const cst_val *rules;
} cst_lts_rewrites;

cst_val *lts_rewrites(const cst_val *itape, const cst_lts_rewrites *r);
cst_val *lts_rewrites_word(const char *word, const cst_lts_rewrites *r);

#endif

