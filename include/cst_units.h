#ifndef _CST_UNITS_H__
#define _CST_UNITS_H__

#include "cst_file.h"
#include "cst_hrg.h"
#include "cst_sts.h"

cst_utterance *join_units(cst_utterance *utt);

cst_utterance *join_units_windowed(cst_utterance *utt);
cst_utterance *join_units_simple(cst_utterance *utt);
cst_utterance *join_units_modified_lpc(cst_utterance *utt);

cst_utterance *asis_to_pm(cst_utterance *utt);
cst_utterance *f0_targets_to_pm(cst_utterance *utt);
cst_utterance *concat_units(cst_utterance *utt);

void add_residual(int targ_size, unsigned char *targ_residual,
		  int unit_size, const unsigned char *unit_residual);
void add_residual_pulse(int targ_size, unsigned char *targ_residual,
			int unit_size, const unsigned char *unit_residual);

#endif
