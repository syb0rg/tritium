set(HRG_SOURCE_FILES
	cst_utterance.c 
	cst_relation.c 
	cst_item.c 
	cst_ffeature.c
	cst_rel_io.c
)
add_library(hrg OBJECT ${HRG_SOURCE_FILES})