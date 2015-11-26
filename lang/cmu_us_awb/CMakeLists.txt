set(CMU_US_AWB_SOURCE_FILES
	cmu_us_awb.c 
	cmu_us_awb_cg_single_mcep_trees.c
       	cmu_us_awb_cg.c 
	cmu_us_awb_cg_single_params.c
       	cmu_us_awb_cg_durmodel.c 
	cmu_us_awb_cg_phonestate.c
       	cmu_us_awb_cg_f0_trees.c
)
add_library(cmu_us_awb OBJECT ${CMU_US_AWB_SOURCE_FILES})