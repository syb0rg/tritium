#include "cst_item.h"
#include "cst_relation.h"
#include "cst_utterance.h"

static const char * const cst_relation_noname = "NoName";

cst_relation *new_relation(const char *name, cst_utterance *u)
{
    cst_relation *r = cst_utt_alloc(u,cst_relation,1);

    r->name = cst_strdup(name);
    r->features = new_features_local(u->ctx);
    r->head = NULL;
    r->utterance = u;

    return r;
}

void delete_relation(cst_relation *r)
{
    cst_item *p, *np;

    if (r != NULL)
    {
	/* This needs to traverse the *all* items */
	for(p=r->head; p; p=np)
	{
	    np = item_next(p);
	    delete_item(p);  /* this *does* go down daughters too */
	}
	delete_features(r->features);
	cst_free(r->name);
	cst_utt_free(r->utterance,r);
    }
}

cst_item *relation_head(cst_relation *r)
{
    return ( r == NULL ? NULL : r->head);
}

cst_item *relation_tail(cst_relation *r)
{
    return (r == NULL ? NULL : r->tail);
}

const char *relation_name(cst_relation *r)
{
    return (r == NULL ? cst_relation_noname : r->name);
}

cst_item *relation_append(cst_relation *r, cst_item *i)
{
    cst_item *ni = new_item_relation(r,i);
    
    if (r->head == NULL)
	r->head = ni;

    ni->p = r->tail;
    if (r->tail)
	r->tail->n = ni;
    r->tail = ni;
    return ni;
}

cst_item *relation_prepend(cst_relation *r, cst_item *i)
{
    cst_item *ni = new_item_relation(r,i);
    
    if (r->tail == NULL)
	r->tail = ni;

    ni->n = r->head;
    if (r->head)
	r->head->p = ni;
    r->head = ni;
    return ni;
}
