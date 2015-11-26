set(CMU_US_SLT_SOURCE_FILES
	cmu_us_slt.c 
	cmu_us_slt_cg_single_mcep_trees.c
       	cmu_us_slt_cg.c 
	cmu_us_slt_cg_single_params.c
       	cmu_us_slt_cg_durmodel.c 
	cmu_us_slt_cg_phonestate.c
       	cmu_us_slt_cg_f0_trees.c
)
add_library(cmu_us_slt OBJECT ${CMU_US_SLT_SOURCE_FILES})