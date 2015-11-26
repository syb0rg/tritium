set(STATS_SOURCE_FILES
	cst_cart.c 
	cst_viterbi.c 
	cst_ss.c
)
add_library(stats OBJECT ${STATS_SOURCE_FILES})