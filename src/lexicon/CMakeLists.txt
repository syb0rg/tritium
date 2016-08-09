set(LEXICON_SOURCE_FILES
	cst_lexicon.c 
	cst_lts.c 
	cst_lts_rewrites.c
)
add_library(lexicon OBJECT ${LEXICON_SOURCE_FILES})
target_compile_features(lexicon PRIVATE c_restrict)

if (DEPENDENCIES)
    add_dependencies(lexicon ${DEPENDENCIES})
endif ()