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
target_compile_features(synth PRIVATE c_restrict)

if (DEPENDENCIES)
    add_dependencies(synth ${DEPENDENCIES})
endif ()