set(CMULEX_SOURCE_FILES
 	cmu_lts_rules.c
	cmu_lts_model.c
       	cmu_lex.c 
	cmu_lex_entries.c 
	cmu_lex_data.c 
	cmu_postlex.c
)
add_library(cmulex OBJECT ${CMULEX_SOURCE_FILES})