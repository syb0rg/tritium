#include "cst_synth.h"
#include "cst_utt_utils.h"

cst_wave *utt_wave(cst_utterance *u)
{
    if (u)
	return val_wave(feat_val(u->features,"wave"));
    else
	return 0;
}

int utt_set_wave(cst_utterance *u, cst_wave *w)
{
    feat_set(u->features,"wave",wave_val(w));
    return 0;
}

const char *utt_input_text(cst_utterance *u)
{
    return val_string(feat_val(u->features,"input_text"));
}

int utt_set_input_text(cst_utterance *u,const char *text)
{
    feat_set_string(u->features,"input_text",text);
    return 0;
}
