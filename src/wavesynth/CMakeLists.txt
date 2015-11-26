set(WAVESYNTH_SOURCE_FILES
	cst_units.c 
	cst_clunits.c
	cst_diphone.c
	cst_sigpr.c
	cst_sts.c
	cst_reflpc.c
)
add_library(wavesynth OBJECT ${WAVESYNTH_SOURCE_FILES})