#ifndef _CST_SIGPR_H__
#define _CST_SIGPR_H__

#include "cst_file.h"
#include "cst_val.h"
#include "cst_sts.h"

cst_wave *lpc_resynth(cst_lpcres *lpcres);
cst_wave *lpc_resynth_fixedpoint(cst_lpcres *lpcres);
cst_wave *lpc_resynth_spike(cst_lpcres *lpcres);

#endif
