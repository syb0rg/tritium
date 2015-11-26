#include "tritium.h"
#include "cst_tokenstream.h"

static const char *ssml_whitespacesymbols;
static const char *ssml_singlecharsymbols_general = "<>&/\";";
static const char *ssml_singlecharsymbols_inattr = "=>;/\"";

static void add_raw_data(cst_utterance *u, const char *raw_data,
			 cst_features *attributes)
{
    /* Add all tokens in raw _data to u */
    cst_tokenstream *ts;
    cst_relation *r;
    cst_item *t;
    const char *token;

    r = utt_relation_create(u,"Token");
    ts = 
     ts_open_string(raw_data,
                    get_param_string(u->features,"text_whitespace",NULL),
                    get_param_string(u->features,"text_singlecharsymbols",NULL),
                    get_param_string(u->features,"text_prepunctuation",NULL),
                    get_param_string(u->features,"text_pospunctuation",NULL));
    while (!(ts_eof(ts)))
    {
	t = relation_append(r,NULL);
	feat_copy_into(item_feats(t),attributes);
	token = ts_get(ts);
	if (cst_strlen(token) > 0)
	{
	    t = relation_append(r,NULL);
	    item_set_string(t,"name",token);
	    item_set_string(t,"whitespace",ts->whitespace);
	    item_set_string(t,"prepunctuation",ts->prepunctuation);
	    item_set_string(t,"punc",ts->postpunctuation);
	}
    }

}

static const char *ts_get_quoted_remainder(cst_tokenstream *ts)
{
    const char *q;

    q = ts_get_quoted_token(ts,'"','\\');

    return q;
}

static cst_features *ssml_get_attributes(cst_tokenstream *ts)
{
    cst_features *a = new_features();
    const char* name, *val;

    set_charclasses(ts,
                    ts->p_whitespacesymbols,
                    ssml_singlecharsymbols_inattr,
                    ts->p_prepunctuationsymbols,
                    ts->p_postpunctuationsymbols);

    name = ts_get(ts);
    while (!streq(">",name))
    {
	if (streq(name,"/"))
	    feat_set_string(a,"_type","startend");
	else
	{
	    feat_set_string(a,"_type","start");
	    feat_set_string(a,"_name0",name);
	    if (streq("=",ts_get(ts)))
	    {
                val = ts_get_quoted_remainder(ts);
                feat_set_string(a,"_val0",val);
            }
	}
	if (ts_eof(ts))
	{
	    fprintf(stderr,"ssml: unexpected EOF\n");
	    delete_features(a);
	    return 0;
	}
        name = ts_get(ts);
    }
	
    set_charclasses(ts,
                    ts->p_whitespacesymbols,
                    ssml_singlecharsymbols_general,
                    ts->p_prepunctuationsymbols,
                    ts->p_postpunctuationsymbols);

    return a;
}

static cst_utterance *ssml_apply_tag(const char *tag,
                                     cst_features *attributes,
                                     cst_utterance *u,
                                     cst_features *word_feats)
{
    printf("SSML TAG %s\n",tag);
    feat_print(stdout,attributes);
    printf("...\n");

    /* do stuff */
    /* flag what to do mark or end */
    /*
      ph set attributes silence all contained tokens
      break add to previous token a break marker
      audio silence all following tokens (utt break)
        insert waveform 

    */

    return u;
}
			       
static void extend_buffer(char **buffer,int *buffer_max,int at_least)
{
    int new_max;

    new_max = (*buffer_max)+at_least;
    cst_free(*buffer);
    *buffer = cst_alloc(char,new_max);
    *buffer_max = new_max;
}			  

static float tritium_ssml_to_speech_ts(cst_tokenstream *ts,
                                     cst_voice *voice,
                                     const char *outtype)
{
    cst_features *ssml_feats, *ssml_word_feats;
    cst_features *attributes;
    const char *token;
    char *tag;
    cst_utterance *utt;
    cst_relation *tokrel;
    int num_tokens;
    cst_breakfunc breakfunc = default_utt_break;
    cst_uttfunc utt_user_callback = 0;
    float durs = 0.0;
    cst_item *t;

    ssml_feats = new_features();
    ssml_word_feats = new_features();
    set_charclasses(ts,
                    " \t\n\r",
                    ssml_singlecharsymbols_general,
                    get_param_string(voice->features,"text_prepunctuation",""),
                    get_param_string(voice->features,"text_postpunctuation","")
                    );

    if (feat_present(voice->features,"utt_break"))
	breakfunc = val_breakfunc(feat_val(voice->features,"utt_break"));

    if (feat_present(voice->features,"utt_user_callback"))
	utt_user_callback = val_uttfunc(feat_val(voice->features,"utt_user_callback"));

    num_tokens = 0;
    utt = new_utterance();
    tokrel = utt_relation_create(utt, "Token");
    while (!ts_eof(ts) || num_tokens > 0)
    {
	token = ts_get(ts);
	if (streq("<",token))
	{   /* A tag */
	    tag = cst_upcase(ts_get(ts));
            if (streq("/",tag)) /* an end tag */
            {
                tag = cst_upcase(ts_get(ts));
                attributes = ssml_get_attributes(ts);
                feat_set_string(attributes,"_type","end");
            }
            else
                attributes = ssml_get_attributes(ts);
	    utt = ssml_apply_tag(tag,attributes,utt,ssml_word_feats);
	    cst_free(tag);
	}
	else if (streq("&",token))
	{   /* an escape sequence */
	    /* skip to ; and insert value in rawdata */
	}
        else
        {
            if ((cst_strlen(token) == 0) ||
                (num_tokens > 500) ||  /* need an upper bound */
                (relation_head(tokrel) && 
                 breakfunc(ts,token,tokrel)))
            {
                /* An end of utt, so synthesize it */
                if (utt_user_callback)
                    utt = (utt_user_callback)(utt);
                
                if (utt)
                {
                    utt = tritium_doSynth(utt,voice,utt_synth_tokens);
                    durs += tritium_processOutput(utt,outtype,TRUE);
                    delete_utterance(utt); utt = NULL;
                }
                else 
                    break;

                if (ts_eof(ts)) break;
                
                utt = new_utterance();
                tokrel = utt_relation_create(utt, "Token");
                num_tokens = 0;
            }

            num_tokens++;

            t = relation_append(tokrel, NULL);
            item_set_string(t,"name",token);
            item_set_string(t,"whitespace",ts->whitespace);
            item_set_string(t,"prepunctuation",ts->prepunctuation);
            item_set_string(t,"punc",ts->postpunctuation);
            /* Mark it at the beginning of the token */
            item_set_int(t,"file_pos",
                         ts->file_pos-(1+ /* as we are already on the next char */
                                       cst_strlen(token)+
                                       cst_strlen(ts->prepunctuation)+
                                       cst_strlen(ts->postpunctuation)));
            item_set_int(t,"line_number",ts->line_number);
        }
    }

    delete_utterance(utt);
    return durs;
}

float tritium_ssml_to_speech(const char *filename,
                           cst_voice *voice,
                           const char *outtype)
{
    cst_tokenstream *ts;
    int fp;
    cst_wave *w;
    float d;

    if ((ts = ts_open(filename,
	      get_param_string(voice->features,"text_whitespace",NULL),
	      get_param_string(voice->features,"text_singlecharsymbols",NULL),
	      get_param_string(voice->features,"text_prepunctuation",NULL),
	      get_param_string(voice->features,"text_postpunctuation",NULL)))
	== NULL)
    {
	cst_errmsg("failed to open file \"%s\" for ssml reading\n",
		   filename);
	return 1;
    }
    fp = get_param_int(voice->features,"file_start_position",0);
    if (fp > 0)
        ts_set_stream_pos(ts,fp);

    /* If its a file to write to, create and save an empty wave file */
    /* as we are going to incrementally append to it                 */
    if (!streq(outtype,"play") && 
        !streq(outtype,"none") &&
        !streq(outtype,"stream"))
    {
	w = new_wave();
	cst_wave_resize(w,0,1);
	cst_wave_set_sample_rate(w,16000);
	cst_wave_save_riff(w,outtype);  /* an empty wave */
	delete_wave(w);
    }

    d = tritium_ssml_to_speech_ts(ts,voice,outtype);

    ts_close(ts);
    
    return d;

}


