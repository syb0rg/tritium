set(REGEX_SOURCE_FILES
	cst_regex.c 
	regexp.c 
	regsub.c
)
add_library(regex OBJECT ${REGEX_SOURCE_FILES})
target_compile_features(regex PRIVATE c_restrict)

if (DEPENDENCIES)
    add_dependencies(regex ${DEPENDENCIES})
endif ()