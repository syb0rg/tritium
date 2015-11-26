#ifndef _CST_VOICE_H__
#define _CST_VOICE_H__

#include "cst_file.h"
#include "cst_val.h"
#include "cst_features.h"
#include "cst_utterance.h"
#include "cst_relation.h"

struct cst_voice_struct {
    const char *name;

    cst_features *features;
    cst_features *ffunctions;

    /* This hook is called (from utt_init()) after the input text (if
       any) has been set and voice features have been copied in, but
       before synthesis. */
    cst_utterance *(*utt_init)(cst_utterance *u,
			       struct cst_voice_struct *v);
};
typedef struct cst_voice_struct cst_voice;

/* Constructor functions */
cst_voice *new_voice();
void delete_voice(cst_voice *u);

CST_VAL_USER_TYPE_DCLS(voice,cst_voice)

#endif
