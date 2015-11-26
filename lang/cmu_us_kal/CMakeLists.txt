set(CMU_US_KAL_SOURCE_FILES
	cmu_us_kal_diphone.c 
	cmu_us_kal.c
       	cmu_us_kal_lpc.c
      	cmu_us_kal_res.c
       	cmu_us_kal_residx.c
)
add_library(cmu_us_kal OBJECT ${CMU_US_KAL_SOURCE_FILES})