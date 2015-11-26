#include <string.h>
#include "tritium.h"
#include "cst_clunits.h"
#include "usenglish.h"
#include "cmu_lex.h"

int cmu_syl_boundary(const cst_item *i,const cst_val *rest);

static char *cmu_time_awb_unit_name(cst_item *s);

extern cst_clunit_db cmu_time_awb_db;

extern const int cmu_time_awb_lex_entry[];
extern const unsigned char cmu_time_awb_lex_data[];
extern const int cmu_time_awb_num_entries;
extern const int cmu_time_awb_num_bytes;
extern const char * const cmu_time_awb_lex_phone_table[54];
extern const char * const cmu_time_awb_lex_phones_huff_table[];
extern const char * const cmu_time_awb_lex_entries_huff_table[];

cst_lexicon cmu_time_awb_lex;

cst_voice *cmu_time_awb_ldom = NULL;

cst_voice *register_cmu_time_awb(const char *voxdir)
{
    cst_voice *v = new_voice();

    v->name = "awb_time";

    /* Sets up language specific parameters in the voice. */
    usenglish_init(v);

    /* Things that weren't filled in already. */
    tritium_featSetString(v->features,"name","awb");

    /* Lexicon */
    cmu_time_awb_lex.name = "cmu_time_awb";
    cmu_time_awb_lex.num_entries = cmu_time_awb_num_entries;
    cmu_time_awb_lex.num_bytes = cmu_time_awb_num_bytes;
    cmu_time_awb_lex.data = cmu_time_awb_lex_data;
    cmu_time_awb_lex.phone_table = cmu_time_awb_lex_phone_table;
    cmu_time_awb_lex.syl_boundary = cmu_syl_boundary;
    cmu_time_awb_lex.lts_rule_set = NULL;
    cmu_time_awb_lex.phone_hufftable = cmu_time_awb_lex_phones_huff_table;
    cmu_time_awb_lex.entry_hufftable = cmu_time_awb_lex_entries_huff_table;
    
    tritium_featSet(v->features,"lexicon",lexicon_val(&cmu_time_awb_lex));

    /* Waveform synthesis */
    tritium_featSet(v->features,"wave_synth_func",uttfunc_val(&clunits_synth));
    tritium_featSet(v->features,"clunit_db",clunit_db_val(&cmu_time_awb_db));
    tritium_featSetInt(v->features,"sample_rate",cmu_time_awb_db.sts->sample_rate);
    tritium_featSetString(v->features,"join_type","simple_join");
    tritium_featSetString(v->features,"resynth_type","fixed");

    /* Unit selection */
    cmu_time_awb_db.unit_name_func = cmu_time_awb_unit_name;

    cmu_time_awb_ldom = v;

    return cmu_time_awb_ldom;
}

void unregister_cmu_time_awb(cst_voice *vox)
{
    if (vox != cmu_time_awb_ldom)
	return;
    delete_voice(vox);
    cmu_time_awb_ldom = NULL;
}

static char *cmu_time_awb_unit_name(cst_item *s)
{
	return clunits_ldom_phone_word(s);
}
