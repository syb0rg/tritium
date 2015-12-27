#ifndef _CST_CLUNITS_H__
#define _CST_CLUNITS_H__

#include "cst_wave.h"
#include "cst_cart.h"
#include "cst_viterbi.h"
#include "cst_track.h"
#include "cst_sts.h"

#define CLUNIT_NONE (unsigned short)65535

typedef struct cst_clunit_struct {
    unsigned short type, phone;
    int start, end;
    int prev, next;
} cst_clunit;

typedef struct cst_clunit_type_struct {
    const char *name;
    int start, count;
} cst_clunit_type;

typedef struct cst_clunit_db_struct {
    const char *name;
    const cst_clunit_type *types;
    const cst_cart * const *trees; 
    const cst_clunit *units;
    int num_types, num_units;

    /* These may be set up at runtime (in file-mapped databases) */
    cst_sts_list *sts, *mcep;

    /* These are pre-scaled by 65536 to accomodate fixed-point machines */
    const int *join_weights;

    /* Misc. important parameters */
    int optimal_coupling;
    int extend_selections;
    int f0_weight;
    char *(*unit_name_func)(cst_item *s);
} cst_clunit_db;

CST_VAL_USER_TYPE_DCLS(clunit_db,cst_clunit_db)
CST_VAL_USER_TYPE_DCLS(vit_cand,cst_vit_cand)

cst_utterance *clunits_synth(cst_utterance *utt);
cst_utterance *clunits_dump_units(cst_utterance *utt);

char *clunits_ldom_phone_word(cst_item *s);
int clunit_get_unit_index(cst_clunit_db *cludb,
			  const char *unit_type,
			  int instance);
int clunit_get_unit_index_name(cst_clunit_db *cludb,
			       const char *name);

#define UNIT_TYPE(db,u) ((db)->types[(db)->units[(u)].type].name)
#define UNIT_INDEX(db,u) ((u) - (db)->types[(db)->units[(u)].type].start)

#endif
