#ifndef _CST_ARGS_H__
#define _CST_ARGS_H__

#include "cst_features.h"

cst_val *cst_args(char **argv, int argc,
		  const char *description,
		  cst_features *args);

#endif
