#ifndef _SYNTH_H__
#define _SYNTH_H__

#include "cst_hrg.h"
#include "cst_tokenstream.h"
#include "cst_voice.h"

typedef int (*cst_breakfunc)(cst_tokenstream *ts, 
		             const char *token, 
			     cst_relation *tokens);
CST_VAL_USER_FUNCPTR_DCLS(breakfunc,cst_breakfunc)
int default_utt_break(cst_tokenstream *ts,
		      const char *token, cst_relation *tokens);

/* You must call utt_init before any of the others. */
cst_utterance *utt_init(cst_utterance *u, cst_voice *vox);
cst_utterance *utt_synth(cst_utterance *u);
cst_utterance *utt_synth_phones(cst_utterance *u);
cst_utterance *utt_synth_tokens(cst_utterance *u);

typedef struct cst_dur_stats_struct {
    char *phone;
    float mean;
    float stddev;
} dur_stat;
typedef dur_stat *dur_stats; /* only one star, due to funky cst_val magic */
CST_VAL_USER_TYPE_DCLS(dur_stats,dur_stats)

cst_utterance *default_segmentanalysis(cst_utterance *u);

cst_utterance *default_tokenization(cst_utterance *u);
cst_utterance *default_textanalysis(cst_utterance *u);
cst_val *default_tokentowords(cst_item *i);
cst_utterance *default_phrasing(cst_utterance *u);
cst_utterance *default_pos_tagger(cst_utterance *u);
cst_utterance *default_lexical_insertion(cst_utterance *u);
cst_utterance *default_pause_insertion(cst_utterance *u);

cst_utterance *cart_intonation(cst_utterance *u);
cst_utterance *cart_duration(cst_utterance *u);

cst_utterance *flat_prosody(cst_utterance *u);

typedef struct cst_synth_module_struct {
    const char *hookname;
    cst_uttfunc defhook;
} cst_synth_module;

cst_utterance *apply_synth_module(cst_utterance *u,
				  const cst_synth_module *mod);
cst_utterance *apply_synth_method(cst_utterance *u,
				  const cst_synth_module meth[]);
#endif

