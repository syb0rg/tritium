#ifndef _US_ENGLISH_H_
#define _US_ENGLISH_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cst_utterance.h"
#include "cst_cart.h"
#include "cst_val.h"
#include "cst_phoneset.h"
#include "cst_lexicon.h"
#include "cst_synth.h"

/* Voices call this to use usenglish. */
void usenglish_init(cst_voice *v);

/* Default functions and values that you might need. */
cst_utterance *us_f0_model(cst_utterance *u);
extern const cst_phoneset us_phoneset;
extern const cst_cart us_phrasing_cart;
extern const cst_cart us_int_accent_cart;
extern const cst_cart us_int_tone_cart;
extern const cst_cart us_durz_cart;
extern const cst_cart us_pos_cart;
extern const dur_stat * const us_dur_stats[];

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif
