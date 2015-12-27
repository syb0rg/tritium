#ifndef _UTT_UTILS_H__
#define _UTT_UTILS_H__

#include "cst_hrg.h"
#include "cst_wave.h"

int utt_set_wave(cst_utterance *u, cst_wave *w);
cst_wave *utt_wave(cst_utterance *u);

const char *utt_input_text(cst_utterance *u);
int utt_set_input_text(cst_utterance *u,const char *text);

#define utt_feat_string(U,F) (feat_string((U)->features,F))
#define utt_feat_int(U,F) (feat_int((U)->features,F))
#define utt_feat_float(U,F) (feat_float((U)->features,F))
#define utt_feat_val(U,F) (feat_val((U)->features,F))

#define utt_set_feat_string(U,F,V) (feat_set_string((U)->features,F,V))
#define utt_set_feat_int(U,F,V) (feat_set_int((U)->features,F,V))
#define utt_set_feat_float(U,F,V) (feat_set_float((U)->features,F,V))
#define utt_set_feat(U,F,V) (feat_set((U)->features,F,V))

#define utt_rel_head(U,R) (relation_head(utt_relation((U),R)))

#endif
