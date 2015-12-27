#ifndef _CST_RELATION_H__
#define _CST_RELATION_H__

#include "cst_file.h"
#include "cst_val.h"
#include "cst_features.h"
#include "cst_item.h"
#include "cst_utterance.h"

struct cst_relation_struct {
    char *name;
    cst_features *features;
    cst_utterance *utterance;
    cst_item *head;
    cst_item *tail;
};

/* Constructor functions */
cst_relation *new_relation(const char *name, cst_utterance *u);

void delete_relation(cst_relation *r);

cst_item *relation_head(cst_relation *r);
cst_item *relation_tail(cst_relation *r);
const char *relation_name(cst_relation *r);

cst_item *relation_append(cst_relation *r,cst_item *i);
cst_item *relation_prepend(cst_relation *r,cst_item *i);

int relation_load(cst_relation *r, const char *filename);
int relation_save(cst_relation *r, const char *filename);

#endif
