#include "tritium.h"
#include "cst_diphone.h"
#include "usenglish.h"
#include "cmu_lex.h"

static cst_utterance *cmu_us_kal_postlex(cst_utterance *u);
extern cst_diphone_db cmu_us_kal_db;

cst_voice *cmu_us_kal_diphone = NULL;

cst_voice *register_cmu_us_kal(const char *voxdir)
{
    cst_voice *v;
    cst_lexicon *lex;

    if (cmu_us_kal_diphone)
        return cmu_us_kal_diphone;  /* Already registered */
    
    v  = new_voice();
    v->name = "kal";

    /* Set up basic values for synthesizing with this voice */
    usenglish_init(v);
    tritium_featSetString(v->features,"name","cmu_us_kal_diphone");

    /* Lexicon */
    lex = cmu_lex_init();
    tritium_featSet(v->features,"lexicon",lexicon_val(lex));

    /* Intonation */
    tritium_featSetFloat(v->features,"int_f0_target_mean",95.0);
    tritium_featSetFloat(v->features,"int_f0_target_stddev",11.0);

    tritium_featSetFloat(v->features,"duration_stretch",1.1); 

    /* Post lexical rules */
    tritium_featSet(v->features,"postlex_func",uttfunc_val(&cmu_us_kal_postlex));

    /* Waveform synthesis: diphone_synth */
    tritium_featSet(v->features,"wave_synth_func",uttfunc_val(&diphone_synth));
    tritium_featSet(v->features,"diphone_db",diphone_db_val(&cmu_us_kal_db));
    tritium_featSetInt(v->features,"sample_rate",cmu_us_kal_db.sts->sample_rate);
    tritium_featSetString(v->features,"resynth_type","fixed");
    tritium_featSetString(v->features,"join_type","modified_lpc");

    cmu_us_kal_diphone = v;

    return cmu_us_kal_diphone;
}

void unregister_cmu_us_kal(cst_voice *v)
{
    if (v != cmu_us_kal_diphone)
	return;
    delete_voice(v);
    cmu_us_kal_diphone = NULL;
}

static void fix_ah(cst_utterance *u)
{
    /* This should really be done in the index itself */
    const cst_item *s;

    for (s=relation_head(utt_relation(u,"Segment")); s; s=item_next(s))
	if (streq(item_feat_string(s,"name"),"ah"))
	    item_set_string(s,"name","aa");
}

static cst_utterance *cmu_us_kal_postlex(cst_utterance *u)
{
    cmu_postlex(u);
    fix_ah(u);

    return u;
}

