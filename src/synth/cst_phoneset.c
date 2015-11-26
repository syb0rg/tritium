#include "cst_val.h"
#include "cst_utterance.h"
#include "cst_item.h"
#include "cst_phoneset.h"

CST_VAL_REGISTER_TYPE_NODEL(phoneset,cst_phoneset)

cst_phoneset *new_phoneset()
{
    /* These aren't going to be supported dynamically */
    cst_phoneset *v = cst_alloc(struct cst_phoneset_struct,1);

    return v;
}

void delete_phoneset(cst_phoneset *v)
{
    if (v)
    {
	cst_free(v);
    }
}

int phone_id(const cst_phoneset *ps,const char* phonename)
{
    int i;

    for (i=0; i< ps->num_phones; i++)
	if (streq(ps->phonenames[i],phonename))
	    return i;
    /* Wonder if I should print an error here or not */

    return 0;
}

int phone_feat_id(const cst_phoneset *ps,const char* featname)
{
    int i;

    for (i=0; ps->featnames[i]; i++)
	if (streq(ps->featnames[i],featname))
	    return i;

    /* Wonder if I should print an error here or not */
    return 0;
}

const cst_val *phone_feature(const cst_phoneset *ps,
			     const char* phonename,
			     const char *featname)
{
    return ps->featvals[ps->fvtable[phone_id(ps,phonename)]
          		           [phone_feat_id(ps,featname)]];
}

const char *phone_feature_string(const cst_phoneset *ps,
				 const char* phonename,
				 const char *featname)
{
    return val_string(phone_feature(ps,phonename,featname));
}


const cst_phoneset *item_phoneset(const cst_item *p)
{
    return val_phoneset(feat_val(item_utt(p)->features,"phoneset"));
}


