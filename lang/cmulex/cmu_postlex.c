#include "tritium.h"

static void apostrophe_s(cst_utterance *u)
{
    cst_item *s;
    cst_item *schwa;
    cst_phoneset *ps=0;
    const char *pname, *word;

    ps = val_phoneset(feat_val(u->features,"phoneset"));

    for (s=item_next(relation_head(utt_relation(u,"Segment"))); 
	 s; s=item_next(s))
    {
	word = val_string(ffeature(s, "R:SylStructure.parent.parent.name"));
	if (streq("'s", word))
	{
	    pname = item_feat_string(item_prev(s),"name");
	    if ((strchr("fa",*phone_feature_string(ps,pname,"ctype")) != NULL)
		&& (strchr("dbg",
			   *phone_feature_string(ps,pname,"cplace")) == NULL))
		/* needs a schwa */
	    {
		schwa = item_prepend(s,NULL);
		item_set_string(schwa,"name","ax");
		item_prepend(item_as(s,"SylStructure"),schwa);
	    }
	    else if (streq("-",phone_feature_string(ps,pname,"cvox")))
		item_set_string(s,"name","s");
	}
	else if (streq("'ve", word)
		 || streq("'ll", word)
		 || streq("'d", word))
	{
	    if (streq("-",ffeature_string(s,"p.ph_vc")))
	    {
		schwa = item_prepend(s,NULL);
		item_set_string(schwa,"name","ax");
		item_prepend(item_as(s,"SylStructure"),schwa);
	    }
	}
    }

}

static void the_iy_ax(cst_utterance *u)
{
    const cst_item *i;
    const char *word;

    for (i = relation_head(utt_relation(u, "Segment")); i; i = item_next(i))
    {
	if (streq("ax", item_name(i)))
	{
	    word = ffeature_string(i,"R:SylStructure.parent.parent.name");
	    if (streq("the", word)
		&& streq("+", ffeature_string(i,"n.ph_vc")))
		    item_set_string(i, "name", "iy");
	}

    }
}

cst_utterance *cmu_postlex(cst_utterance *u)
{
    /* Post lexical rules */

    apostrophe_s(u);
    the_iy_ax(u);

    return u;
}
