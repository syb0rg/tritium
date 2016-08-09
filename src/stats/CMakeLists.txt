set(STATS_SOURCE_FILES
	cst_cart.c 
	cst_viterbi.c 
	cst_ss.c
)
add_library(stats OBJECT ${STATS_SOURCE_FILES})
target_compile_features(stats PRIVATE c_restrict)

if (DEPENDENCIES)
    add_dependencies(stats ${DEPENDENCIES})
endif ()