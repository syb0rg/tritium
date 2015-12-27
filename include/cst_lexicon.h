#ifndef _CST_LEXICON_H__
#define _CST_LEXICON_H__

#include "cst_item.h"
#include "cst_lts.h"

typedef struct lexicon_struct {
    char *name;
    int num_entries;
    /* Entries are centered around bytes with value 255 */
    /* entries and forward (compressed) pronunciations and backwards */
    /* each are terminated (preceeded in pron case) by 0 */
    /* This saves 4 bytes per entry for an index */
    unsigned char *data; /* the entries and phone strings */
    int num_bytes;       /* the number of bytes in the data */
    char **phone_table;

    cst_lts_rules *lts_rule_set;

    int (*syl_boundary)(const cst_item *i,const cst_val *p);
    
    cst_val *(*lts_function)(const struct lexicon_struct *l, const char *word, const char *pos);

    char ***addenda;
    /* ngram frequency table used for packed entries */
    const char * const *phone_hufftable;
    const char * const *entry_hufftable;

    cst_utterance *(*postlex)(cst_utterance *u);

    cst_val *lex_addenda;  /* For pronunciations added at run time */

} cst_lexicon;

cst_lexicon *new_lexicon();
void delete_lexicon(cst_lexicon *lex);

cst_val *cst_lex_make_entry(const cst_lexicon *lex, 
                            const cst_string *entry);
cst_val *cst_lex_load_addenda(const cst_lexicon *lex, 
                              const char *lexfile);

cst_val *lex_lookup(const cst_lexicon *l, const char *word, const char *pos);
int in_lex(const cst_lexicon *l, const char *word, const char *pos);

CST_VAL_USER_TYPE_DCLS(lexicon,cst_lexicon)

#endif
