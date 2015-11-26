#ifndef _TRITIUM_H__
#define _TRITIUM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cst_string.h"
#include "cst_regex.h"
#include "cst_val.h"
#include "cst_features.h"
#include "cst_item.h"
#include "cst_relation.h"
#include "cst_utterance.h"
#include "cst_wave.h"
#include "cst_track.h"

#include "cst_cart.h"
#include "cst_phoneset.h"
#include "cst_voice.h"
#include "cst_audio.h"

#include "cst_utt_utils.h"
#include "cst_lexicon.h"
#include "cst_synth.h"
#include "cst_units.h"
#include "cst_tokenstream.h"

extern cst_val *tritium_voiceList;

/* Public functions */
int tritium_init();

/* General top level functions */
cst_voice *tritium_voiceSelect(const char *name);
float tritium_fileToSpeech(const char *filename, 
			   cst_voice *voice,
			   const char *outtype);
float tritium_textToSpeech(const char *text, 
			   cst_voice *voice,
			   const char *outtype);
float tritium_phonesToSpeech(const char *text, 
			     cst_voice *voice,
			     const char *outtype);
float tritium_ssml_to_speech(const char *filename,
                           cst_voice *voice,
                           const char *outtype);
int tritium_voiceAddLexAddenda(cst_voice *v, const cst_string *lexfile);

/* Lower lever user functions */
cst_wave *tritium_textToWave(const char *text,cst_voice *voice);
cst_utterance *tritium_synthText(const char *text,cst_voice *voice);
cst_utterance *tritium_synthPhones(const char *phones,cst_voice *voice);

cst_utterance *tritium_doSynth(cst_utterance *u,
                              cst_voice *voice,
                              cst_uttfunc synth);
float tritium_processOutput(cst_utterance *u,
                           const char *outtype,
                           int append);

/* tritium public export wrappers for features access */
int tritium_getParamInt(const cst_features *f, const char *name,int def);
float tritium_getParamFloat(const cst_features *f, const char *name, float def);
const char *tritium_getParamString(const cst_features *f, const char *name, const char *def);
const cst_val *tritium_getParamVal(const cst_features *f, const char *name, cst_val *def);
void tritium_featSetInt(cst_features *f, const char *name, int v);
void tritium_featSetFloat(cst_features *f, const char *name, float v);
void tritium_featSetString(cst_features *f, const char *name, const char *v);
void tritium_featSet(cst_features *f, const char *name,const cst_val *v);
int tritium_featRemove(cst_features *f, const char *name);

const char *tritium_ffeatureString(const cst_item *item,const char *featpath);
int tritium_ffeatureInt(const cst_item *item,const char *featpath);
float tritium_ffeatureFloat(const cst_item *item,const char *featpath);
const cst_val *tritium_ffeature(const cst_item *item,const char *featpath);
cst_item* tritium_pathToItem(const cst_item *item,const char *featpath);

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */


#endif
