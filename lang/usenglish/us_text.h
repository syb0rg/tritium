#ifndef _US_TEXT_H__
#define _US_TEXT_H__

#include "cst_val.h"
#include "cst_hrg.h"
#include "cst_cart.h"

cst_val *en_exp_number(const char *numstring);
cst_val *en_exp_digits(const char *numstring);
cst_val *en_exp_id(const char *numstring);
cst_val *en_exp_ordinal(const char *numstring);
cst_val *en_exp_letters(const char *lets);
cst_val *en_exp_real(const char *numstring);
int en_exp_roman(const char *roman);

extern const cst_cart us_nums_cart;

void us_text_init();
cst_utterance *us_textanalysis(cst_utterance *u);
cst_val *us_tokentowords(cst_item *token);

int us_aswd(const char *w);

#endif

