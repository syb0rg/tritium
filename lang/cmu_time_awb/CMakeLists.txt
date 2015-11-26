set(CMU_TIME_AWB_SOURCE_FILES
	cmu_time_awb.c
	cmu_time_awb_clunits.c
	cmu_time_awb_cart.c
	cmu_time_awb_mcep.c
	cmu_time_awb_lpc.c
	cmu_time_awb_lex_entry.c
)
add_library(cmu_time_awb OBJECT ${CMU_TIME_AWB_SOURCE_FILES})