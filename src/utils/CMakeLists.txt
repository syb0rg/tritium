set(UTIL_SOURCE_FILES
	cst_alloc.c 
	cst_error.c 
	cst_string.c 
	cst_wchar.c
	cst_tokenstream.c 
	cst_val.c 
	cst_features.c
	cst_endian.c 
	cst_socket.c 
	cst_val_const.c
	cst_val_user.c 
	cst_args.c 
	cst_mmap_posix.c
	cst_file_stdio.c
)
add_library(util OBJECT ${UTIL_SOURCE_FILES})