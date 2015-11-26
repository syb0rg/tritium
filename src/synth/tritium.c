#include "cst_tokenstream.h"
#include "tritium.h"

/* This is a global, which isn't ideal, this may change */
/* It is set when tritium_set_voice_list() is called which happens in */
/* tritium_main() */
cst_val *tritium_voiceList = 0;

cst_voice *tritium_voiceSelect(const char *name)
{
    const cst_val *v;
    cst_voice *voice;

    if (tritium_voiceList == NULL) return NULL;  // oops, not good
    if (name == NULL) return val_voice(val_car(tritium_voiceList));

    for (v=tritium_voiceList; v; v=val_cdr(v))
    {
        voice = val_voice(val_car(v));
        if (streq(name,voice->name)) return voice;
    }

    return tritium_voiceSelect(NULL);

}

int tritium_voiceAddLexAddenda(cst_voice *v, const cst_string *lexfile)
{
    /* Add addenda in lexfile to current voice */
    cst_lexicon *lex;
    const cst_val *lex_addenda = NULL;
    cst_val *new_addenda;

    lex = val_lexicon(feat_val(v->features,"lexicon"));
    if (feat_present(v->features, "lex_addenda"))
	lex_addenda = feat_val(v->features, "lex_addenda");

    new_addenda = cst_lex_load_addenda(lex, lexfile);
#if 0
    printf("\naddenda: ");
    val_print(stdout, new_addenda);
    printf("\n");
#endif

    new_addenda = val_append(new_addenda, (cst_val *)lex_addenda);
    if (lex->lex_addenda) delete_val(lex->lex_addenda);
    lex->lex_addenda = new_addenda;

    return 0;
}

cst_utterance *tritium_doSynth(cst_utterance *u, cst_voice *voice, cst_uttfunc synth)
{		       
    utt_init(u, voice);
    if ((*synth)(u) == NULL)
    {
		delete_utterance(u);
		return NULL;
    }
    else return u;
}

cst_utterance *tritium_synthText(const char *text, cst_voice *voice)
{
    cst_utterance *u = new_utterance();

    utt_set_input_text(u,text);
    return tritium_doSynth(u, voice, utt_synth);
}

cst_utterance *tritium_synthPhones(const char *text, cst_voice *voice)
{
    cst_utterance *u = new_utterance();

    utt_set_input_text(u,text);
    return tritium_doSynth(u, voice, utt_synth_phones);
}

cst_wave *tritium_textToWave(const char *text, cst_voice *voice)
{
    cst_utterance *u = tritium_synthText(text,voice);

    if (!u) return NULL;

	cst_wave *w = copy_wave(utt_wave(u));
    delete_utterance(u);
    return w;
}

float tritium_fileToSpeech(const char *filename, cst_voice *voice, const char *outtype)
{
    cst_utterance *utt = new_utterance();
    cst_tokenstream *ts;
    const char *token;
    cst_item *t;
    cst_relation *tokrel = utt_relation_create(utt, "Token");;
    float durs = 0;
    int num_tokens = 0;
    cst_wave *w;
    cst_breakfunc breakfunc = default_utt_break;
    cst_uttfunc utt_user_callback = 0;
    int fp;

    if ((ts = ts_open(filename,
					  get_param_string(voice->features,"text_whitespace",NULL),
					  get_param_string(voice->features,"text_singlecharsymbols",NULL),
					  get_param_string(voice->features,"text_prepunctuation",NULL),
					  get_param_string(voice->features,"text_postpunctuation",NULL)))
					  == NULL)
    {
		cst_errmsg("failed to open file \"%s\" for reading\n", filename);
		return 1;
    }
    fp = get_param_int(voice->features,"file_start_position",0);
    if (fp > 0) ts_set_stream_pos(ts,fp);

    if (feat_present(voice->features,"utt_break")) breakfunc = val_breakfunc(feat_val(voice->features,"utt_break"));

    if (feat_present(voice->features,"utt_user_callback")) utt_user_callback = val_uttfunc(feat_val(voice->features,"utt_user_callback"));

    /* If its a file to write to, and save an empty wave file */
    /* as we are going to incrementally append to it */
    if (!streq(outtype,"play") && !streq(outtype,"none") && !streq(outtype,"stream"))
    {
		w = new_wave();
		cst_wave_resize(w, 0, 1);
		cst_wave_set_sample_rate(w, 16000);
		cst_wave_save_riff(w,outtype);  /* an empty wave */
		delete_wave(w);
    }

    while (!ts_eof(ts) || num_tokens > 0)
    {
		token = ts_get(ts);
		if ((cst_strlen(token) == 0) || (num_tokens > 500) || (relation_head(tokrel) && breakfunc(ts,token,tokrel)))
		{
			/* An end of utt, so synthesize it */
			if (utt_user_callback) utt = (utt_user_callback)(utt);

			if (utt)
			{
				utt = tritium_doSynth(utt, voice, utt_synth_tokens);
				durs += tritium_processOutput(utt, outtype, TRUE);
				delete_utterance(utt);
				utt = NULL;
			}
			else break;

			if (ts_eof(ts)) break;

			utt = new_utterance();
			tokrel = utt_relation_create(utt, "Token");
			num_tokens = 0;
		}
		num_tokens++;

		t = relation_append(tokrel, NULL);
		item_set_string(t, "name", token);
		item_set_string(t, "whitespace", ts->whitespace);
		item_set_string(t, "prepunctuation", ts->prepunctuation);
		item_set_string(t, "punc", ts->postpunctuation);
		// Mark it at the beginning of the token
		item_set_int(t,"file_pos", ts->file_pos-(1 +
												 cst_strlen(token)+
												 cst_strlen(ts->prepunctuation)+
												 cst_strlen(ts->postpunctuation)));
		item_set_int(t,"line_number",ts->line_number);
    }

    delete_utterance(utt);
    ts_close(ts);
    return durs;
}

float tritium_textToSpeech(const char *text, cst_voice *voice, const char *outtype)
{
    cst_utterance *u = tritium_synthText(text, voice);
    float dur = tritium_processOutput(u, outtype, FALSE);

    delete_utterance(u);
    return dur;
}

float tritium_phonesToSpeech(const char *text, cst_voice *voice, const char *outtype)
{
    cst_utterance *u = tritium_synthPhones(text,voice);
    float dur = tritium_processOutput(u,outtype,FALSE);

    delete_utterance(u);
    return dur;
}

float tritium_processOutput(cst_utterance *u, const char *outtype, int append)
{
	if (!u) return 0.0;
    /* Play or save (append) output to output file */
    cst_wave *w = utt_wave(u);
	float dur = (float)w->num_samples/(float)w->sample_rate;

    if (streq(outtype, "play")) play_wave(w);
	else if (streq(outtype, "stream")); // It's already been played so do nothing
    else if (!streq(outtype, "none"))
    {
        if (append) cst_wave_append_riff(w, outtype);
        else cst_wave_save_riff(w, outtype);
    }

    return dur;
}

int tritium_getParamInt(const cst_features *f, const char *name, int def)
{
    return get_param_int(f, name, def);
}
float tritium_getParamFloat(const cst_features *f, const char *name, float def)
{
    return get_param_float(f, name, def);
}
const char *tritium_getParamString(const cst_features *f, const char *name, const char *def)
{
    return get_param_string(f, name, def);
}
const cst_val *tritium_getParamVal(const cst_features *f, const char *name, cst_val *def)
{
    return get_param_val(f, name, def);
}

void tritium_featSetInt(cst_features *f, const char *name, int v)
{
    feat_set_int(f, name, v);
}
void tritium_featSetFloat(cst_features *f, const char *name, float v)
{
    feat_set_float(f, name, v);
}
void tritium_featSetString(cst_features *f, const char *name, const char *v)
{
    feat_set_string(f, name, v);
}
void tritium_featSet(cst_features *f, const char *name,const cst_val *v)
{
    feat_set(f, name, v);
}

int tritium_featRemove(cst_features *f, const char *name)
{
	return feat_remove(f, name);
}

const char *tritium_ffeatureString(const cst_item *item, const char *featpath)
{
    return ffeature_string(item, featpath);
}

int tritium_ffeatureInt(const cst_item *item, const char *featpath)
{
    return ffeature_int(item, featpath);
}
float tritium_ffeatureFloat(const cst_item *item, const char *featpath)
{
    return ffeature_float(item, featpath);
}
const cst_val *tritium_ffeature(const cst_item *item, const char *featpath)
{
    return ffeature(item, featpath);
}
cst_item* tritium_pathToItem(const cst_item *item, const char *featpath)
{
    return path_to_item(item, featpath);
}

