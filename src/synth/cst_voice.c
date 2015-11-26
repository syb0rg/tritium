#include "cst_alloc.h"
#include "cst_voice.h"

CST_VAL_REGISTER_TYPE(voice,cst_voice)

cst_voice *new_voice()
{
    cst_voice *v = cst_alloc(struct cst_voice_struct,1);

    v->features = new_features();
    v->ffunctions = new_features();

    return v;
}

void delete_voice(cst_voice *v)
{
    if (v)
    {
	delete_features(v->features);
	delete_features(v->ffunctions);
        /* do not delete any dbs in the voice */
        /* they are probably const and not deletable -- at least */
        /* that is true at present */
	cst_free(v);
    }
}
