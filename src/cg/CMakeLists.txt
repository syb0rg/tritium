set(CG_SOURCE_FILES
	cst_cg.c
	cst_mlsa.c
	cst_mlpg.c
	cst_vc.c 
)
add_library(cg OBJECT ${CG_SOURCE_FILES})