#include "cst_hrg.h"
#include "cst_phoneset.h"
#include "cst_regex.h"
#include "cst_ffeatures.h"
#include "us_ffeatures.h"

static const cst_val *gpos(const cst_item *word);

DEF_STATIC_CONST_VAL_STRING(val_string_numeric,"numeric");
DEF_STATIC_CONST_VAL_STRING(val_string_number,"number");
DEF_STATIC_CONST_VAL_STRING(val_string_month,"month");
DEF_STATIC_CONST_VAL_STRING(val_string_day,"day");
DEF_STATIC_CONST_VAL_STRING(val_string_other,"_other_");
DEF_STATIC_CONST_VAL_STRING(val_string_a,"a");
DEF_STATIC_CONST_VAL_STRING(val_string_flight,"flight");
DEF_STATIC_CONST_VAL_STRING(val_string_to,"to");

DEF_STATIC_CONST_VAL_STRING(val_string_content,"content");

static const cst_val *gpos(const cst_item *word)
{
    /* Guess at part of speech (function/content) */
    const char *w;
    int s,t;

    w = item_feat_string(word,"name");

    for (s=0; us_gpos[s]; s++)
    {
	for (t=1; us_gpos[s][t]; t++)
	    if (streq(w,val_string(us_gpos[s][t])))
		return us_gpos[s][0];
    }

    return (cst_val *)&val_string_content;
}

static const cst_val *num_digits(const cst_item *token)
{   
    const char *name = item_feat_string(token,"name");

    return val_int_n(cst_strlen(name));
}

static const cst_val *month_range(const cst_item *token)
{   
    int v = item_feat_int(token,"name");

    if ((v > 0) && ( v < 32))
	return VAL_STRING_1;	
    else
	return VAL_STRING_0;	
}

static const cst_val *token_pos_guess(const cst_item *token)
{   
    const char *name = item_feat_string(token,"name");
    char *dc = cst_downcase(name);
    const cst_val *r;

    if (cst_regex_match(cst_rx_digits,dc))
	r = &val_string_numeric;
    else if ((cst_regex_match(cst_rx_double,dc)) ||
	(cst_regex_match(cst_rx_double,dc)))
	r = &val_string_number;
    else if (streq(dc,"jan") ||
	streq(dc,"january") ||
	streq(dc,"feb") ||
	streq(dc,"february") ||
	streq(dc,"mar") ||
	streq(dc,"march") ||
	streq(dc,"apr") ||
	streq(dc,"april") ||
	streq(dc,"may") ||
	streq(dc,"jun") ||
	streq(dc,"june") ||
	streq(dc,"jul") ||
	streq(dc,"july") ||
	streq(dc,"aug") ||
	streq(dc,"august") ||
	streq(dc,"sep") ||
	streq(dc,"sept") ||
	streq(dc,"september") ||
	streq(dc,"oct") ||
	streq(dc,"october") ||
	streq(dc,"nov") ||
	streq(dc,"november") ||
	streq(dc,"dec") ||
	streq(dc,"december"))
	r = &val_string_month;
    else if (streq(dc,"sun") ||
	streq(dc,"sunday") ||
	streq(dc,"mon") ||
	streq(dc,"monday") ||
	streq(dc,"tue") ||
	streq(dc,"tues") ||
	streq(dc,"tuesday") ||
	streq(dc,"wed") ||
	streq(dc,"wednesday") ||
	streq(dc,"thu") ||
	streq(dc,"thurs") ||
	streq(dc,"thursday") ||
	streq(dc,"fri") ||
	streq(dc,"friday") ||
	streq(dc,"sat") ||
	streq(dc,"saturday"))
	r = &val_string_day;
   /* ignoring the "token_most_common" condition, does get used */
    else if (streq(dc,"a"))
	r =  &val_string_a;
    else if (streq(dc,"flight"))
	r =  &val_string_flight;
    else if (streq(dc,"to"))
	r =  &val_string_to;
    else
	r =  &val_string_other;
    cst_free(dc);
    return r;
}

void us_ff_register(cst_features *ffunctions)
{

    /* The language independent ones */
    basic_ff_register(ffunctions);

    ff_register(ffunctions, "gpos",gpos);
    ff_register(ffunctions, "num_digits",num_digits);
    ff_register(ffunctions, "month_range",month_range);
    ff_register(ffunctions, "token_pos_guess",token_pos_guess);

}
