#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "tritium.h"
#include "tritium_version.h"

#define ITER_MAX 3

cst_val *tritium_set_voice_list(void);

void cst_alloc_debug_summary();

static void tritium_version()
{
#warning finish version function
    printf("  version: %s-%s-%s %s\n", TRITIUM_PROJECT_PREFIX, TRITIUM_PROJECT_VERSION, TRITIUM_PROJECT_STATE, TRITIUM_PROJECT_DATE);
}

static void tritium_usage()
{
    printf("tritium: a small simple speech synthesizer\n");
    tritium_version();
    printf("usage: tritium TEXT/FILE [WAVEFILE]\n"
           "  Converts text in TEXTFILE to a waveform in WAVEFILE\n"
           "  If text contains a space the it is treated as a literal\n"
           "  textstring and spoken, and not as a file name\n"
           "  if WAVEFILE is unspecified or \"play\" the result is\n"
           "  played on the current systems audio device.  If WAVEFILE\n"
           "  is \"none\" the waveform is discarded (good for benchmarking)\n"
           "  Other options must appear before these options\n"
           "  --version   Output tritium version number\n"
           "  --help      Output usage string\n"
           "  -o WAVEFILE Explicitly set output filename\n"
           "  -f TEXTFILE Explicitly set input filename\n"
           "  -t TEXT     Explicitly set input textstring\n"
           "  -p PHONES   Explicitly set input textstring and synthesize as phones\n"
           "  --set F=V   Set feature (guesses type)\n"
           "  -s F=V      Set feature (guesses type)\n"
           "  --seti F=V  Set int feature\n"
           "  --setf F=V  Set float feature\n"
           "  --sets F=V  Set string feature\n"
	   "  -ssml       Read input text/file in ssml mode\n"
	   "  -b          Benchmark mode\n"
	   "  -l          Loop endlessly\n"
	   "  -voice NAME Use voice NAME\n"
	   "  -lv         List voices available\n"
	   "  -add_lex FILENAME add lex addenda from FILENAME\n"
	   "  -pw         Print words\n"
	   "  -ps         Print segments\n"
	   "  -pr RelName  Print relation RelName\n"
           "  -v          Verbose mode\n");
    exit(0);
}

static void tritium_voiceList_print(void)
{
    cst_voice *voice;

    printf("Voices available: ");
    for (const cst_val *v = tritium_voiceList; v; v = val_cdr(v))
    {
        voice = val_voice(val_car(v));
        printf("%s ", voice->name);
    }
    printf("\n");
}

static cst_utterance *print_info(cst_utterance *u)
{
    const char *relname = utt_feat_string(u,"print_info_relation");
    for (cst_item *item = relation_head(utt_relation(u, relname)); item; item=item_next(item))
    {
		printf("%s ", item_feat_string(item, "name"));
    }
    printf("\n");

    return u;
}

static void ef_set(cst_features *f, const char *fv, const char *type)
{
    /* set feature from fv (F=V), guesses type if not explicit type given */
    const char *val = strchr(fv,'=');

    if (val == 0) fprintf(stderr, "tritium: can't find '=' in featval \"%s\", ignoring it\n", fv);
    else
    {
		char *feat = cst_strdup(fv);
		feat[cst_strlen(fv) - cst_strlen(val)] = '\0';
		val += 1;
		if ((type && streq("int", type)) || ((type == 0) && (cst_regex_match(cst_rx_int,val)))) feat_set_int(f,feat,atoi(val));
		else if ((type && streq("float",type)) || ((type == 0) && (cst_regex_match(cst_rx_double,val)))) feat_set_float(f,feat,atof(val));
		else feat_set_string(f,feat,val);
		/* I don't free feat, because feats think featnames are const */
		/* which is true except in this particular case          */
    }
}

int main(int argc, char **argv)
{
    struct timeval tv;
    cst_voice *v;
    const char *filename = 0;
    const char *outtype = "play"; // default is to play
    cst_voice *desired_voice = 0;
    float durs;
    double time_start, time_end;
	int tritium_verbose = FALSE;
	int tritium_loop = FALSE;
	int tritium_bench = FALSE;
	int explicit_filename = FALSE;
	int explicit_text = FALSE;
	int explicit_phones = FALSE;
	int ssml_mode = FALSE;
	int bench_iter = 0;
    cst_features *extra_feats = new_features();
    const char *lex_addenda_file = NULL;
    cst_audio_streaming_info *asi;

    tritium_voiceList = tritium_set_voice_list();

    for (int i = 1; i < argc; i++)
    {
		if (streq(argv[i], "--version"))
		{
			tritium_version();
			return 1;
		}
		else if (streq(argv[i], "-h") || streq(argv[i], "--help") || streq(argv[i], "-?")) tritium_usage();
		else if (streq(argv[i], "-v")) tritium_verbose = TRUE;
		else if (streq(argv[i], "-lv"))
		{
			tritium_voiceList_print();
			exit(0);
		}
		else if (streq(argv[i], "-l")) tritium_loop = TRUE;
		else if (streq(argv[i], "-b"))
		{
			tritium_bench = TRUE;
			break; // ignore other arguments
		}
		else if ((streq(argv[i], "-o")) && (i + 1 < argc))
		{
			outtype = argv[i + 1];
			i++;
		}
		else if ((streq(argv[i], "-voice")) && (i + 1 < argc))
		{
			desired_voice = tritium_voiceSelect(argv[i+1]);
			i++;
		}
		else if ((streq(argv[i],"-add_lex")) && (i+1 < argc))
		{
			lex_addenda_file = argv[i+1];
			i++;
		}
		else if (streq(argv[i], "-f") && (i + 1 < argc))
		{
			filename = argv[i+1];
			explicit_filename = TRUE;
			i++;
		}
		else if (streq(argv[i], "-pw"))
		{
			feat_set_string(extra_feats, "print_info_relation", "Word");
			feat_set(extra_feats, "post_synth_hook_func", uttfunc_val(&print_info));
		}
		else if (streq(argv[i],"-ps"))
		{
			feat_set_string(extra_feats,"print_info_relation","Segment");
			feat_set(extra_feats,"post_synth_hook_func", uttfunc_val(&print_info));
		}
		else if (streq(argv[i],"-ssml"))
		{
			ssml_mode = TRUE;
		}
		else if (streq(argv[i],"-pr") && (i+1 < argc))
		{
			feat_set_string(extra_feats,"print_info_relation",argv[i+1]);
			feat_set(extra_feats,"post_synth_hook_func", uttfunc_val(&print_info));
			i++;
		}
		else if ((streq(argv[i],"-set") || streq(argv[i],"-s")) && (i+1 < argc))
		{
			ef_set(extra_feats,argv[i+1],0);
			i++;
		}
		else if (streq(argv[i],"--seti") && (i + 1 < argc))
		{
			ef_set(extra_feats,argv[i+1],"int");
			i++;
		}
		else if (streq(argv[i],"--setf") && (i + 1 < argc))
		{
			ef_set(extra_feats,argv[i+1],"float");
			i++;
		}
		else if (streq(argv[i],"--sets") && (i + 1 < argc))
		{
			ef_set(extra_feats,argv[i+1],"string");
			i++;
		}
		else if (streq(argv[i],"-p") && (i + 1 < argc))
		{
			filename = argv[i + 1];
			explicit_phones = TRUE;
			i++;
		}
		else if (streq(argv[i],"-t") && (i + 1 < argc))
		{
			filename = argv[i+1];
			explicit_text = TRUE;
			i++;
		}
		else if (filename) outtype = argv[i];
		else filename = argv[i];
    }

    if (filename == NULL) filename = "-";  /* stdin */
    if (desired_voice == 0) desired_voice = tritium_voiceSelect(NULL);

    v = desired_voice;
    feat_copy_into(extra_feats,v->features);
    durs = 0.0;

    if (lex_addenda_file) tritium_voiceAddLexAddenda(v,lex_addenda_file);

    if (streq("stream",outtype))
    {
        asi = new_audio_streaming_info();
        asi->asc = audio_stream_chunk;
        feat_set(v->features,"streaming_info",audio_streaming_info_val(asi));
    }

    if (tritium_bench)
    {
		outtype = "none";
		filename = "A whole joy was reaping, but they've gone south, you should fetch azure mike.";
		explicit_text = TRUE;
    }

	do
	{
		gettimeofday(&tv, NULL);
		time_start = (double)(tv.tv_sec) + (((double)tv.tv_usec) / 1000000.0);

		if (explicit_phones) durs = tritium_phonesToSpeech(filename,v,outtype);
		else if (ssml_mode) durs = tritium_ssml_to_speech(filename,v,outtype);
		else if ((strchr(filename,' ') && !explicit_filename) || explicit_text) durs = tritium_textToSpeech(filename,v,outtype);
		else durs = tritium_fileToSpeech(filename, v, outtype);

		gettimeofday(&tv, NULL);
		time_end = ((double)(tv.tv_sec)) + ((double)tv.tv_usec / 1000000.0);

		if (tritium_verbose || (tritium_bench && bench_iter == ITER_MAX)) printf("times faster than real-time: %f\n(%f seconds of speech synthesized in %f)\n", durs / (float)(time_end-time_start), durs, (float)(time_end-time_start));
	} while (tritium_loop || (tritium_bench && bench_iter++ < ITER_MAX));

    delete_features(extra_feats);
    delete_val(tritium_voiceList); tritium_voiceList=0;
}
