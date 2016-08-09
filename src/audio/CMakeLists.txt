set(AUDIO_SOURCE_FILES
	auclient.c 
	auserver.c 
	audio.c 
	au_streaming.c
	au_none.c
)

add_library(audio OBJECT ${AUDIO_SOURCE_FILES})
target_compile_features(audio PRIVATE c_restrict)

if (DEPENDENCIES)
    add_dependencies(audio ${DEPENDENCIES})
endif ()
