#ifndef _CMU_LEX_H_
#define _CMU_LEX_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cst_lexicon.h"
cst_lexicon *cmu_lex_init(void);
cst_utterance *cmu_postlex(cst_utterance *u);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

