set(CMU_US_KAL16_SOURCE_FILES
	cmu_us_kal16_diphone.c 
	cmu_us_kal16.c
       	cmu_us_kal16_lpc.c
       	cmu_us_kal16_res.c
       	cmu_us_kal16_residx.c
)
add_library(cmu_us_kal16 OBJECT ${CMU_US_KAL16_SOURCE_FILES})