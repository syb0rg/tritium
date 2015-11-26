set(SYNTH_SOURCE_FILES
	cst_synth.c 
	cst_utt_utils.c 
	cst_voice.c 
	cst_phoneset.c
       cst_ffeatures.c 
	cst_ssml.c
	tritium.c
)
add_library(synth OBJECT ${SYNTH_SOURCE_FILES})