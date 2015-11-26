set(REGEX_SOURCE_FILES
	cst_regex.c 
	regexp.c 
	regsub.c
)
add_library(regex OBJECT ${REGEX_SOURCE_FILES})