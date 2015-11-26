set(CMU_US_RMS_SOURCE_FILES
	cmu_us_rms.c 
	cmu_us_rms_cg_single_mcep_trees.c
       	cmu_us_rms_cg.c 
	cmu_us_rms_cg_single_params.c
       	cmu_us_rms_cg_durmodel.c 
	cmu_us_rms_cg_phonestate.c
       	cmu_us_rms_cg_f0_trees.c
)
add_library(cmu_us_rms OBJECT ${CMU_US_RMS_SOURCE_FILES})