#ifndef _CST_REGEX_H__
#define _CST_REGEX_H__

#include "cst_file.h"
#include "cst_string.h"

/*
 * The first byte of the regexp internal "program" is actually this magic
 * number; the start node begins in the second byte.
 */
#define	CST_REGMAGIC	0234

typedef struct cst_regex_struct {
    char regstart;		/* Internal use only. */
    char reganch;		/* Internal use only. */
    char *regmust;		/* Internal use only. */
    int regmlen;		/* Internal use only. */
    int regsize;
    char *program;
} cst_regex;

#define CST_NSUBEXP  10
typedef struct cst_regstate_struct {
	const char *startp[CST_NSUBEXP];
	const char *endp[CST_NSUBEXP];
	const char *input;
	const char *bol;
} cst_regstate;

cst_regex *new_cst_regex(const char *str);
void delete_cst_regex(cst_regex *r);

int cst_regex_match(const cst_regex *r, const char *str);
cst_regstate *cst_regex_match_return(const cst_regex *r, const char *str);

/* Internal functions from original HS code */
cst_regex *hs_regcomp(const char *);
cst_regstate *hs_regexec(const cst_regex *, const char *);
void hs_regdelete(cst_regex *);

/* Works similarly to snprintf(3), in that at most max characters are
   written to out, including the trailing NUL, and the return value is
   the number of characters written, *excluding* the trailing NUL.
   Also works similarly to wcstombs(3) in that passing NULL as out
   will count the number of characters that would be written without
   doing any actual conversion, and ignoring max.  So, you could use
   it like this:

   rx = new_cst_regex("\\(.*\\)_\\(.*\\)");
   if ((rs = cst_regex_match_return(rx, "foo_bar")) != NULL) {
   	size_t n;

	n = cst_regsub(rs, "\\1_\\2_quux", NULL, 0) + 1;
	out = cst_alloc(char, n);
	cst_regsub(rs, "\\1_\\2_quux", out, n);
   } */
size_t cst_regsub(const cst_regstate *r, const char *in, char *out, size_t max);

/* Regexps used in text processing (these are latin-alphabet specific
   and to some extent US English-specific) */
extern const cst_regex * const cst_rx_white;
extern const cst_regex * const cst_rx_alpha;
extern const cst_regex * const cst_rx_uppercase;
extern const cst_regex * const cst_rx_lowercase;
extern const cst_regex * const cst_rx_alphanum;
extern const cst_regex * const cst_rx_identifier;
extern const cst_regex * const cst_rx_int;
extern const cst_regex * const cst_rx_double;
extern const cst_regex * const cst_rx_commaint;
extern const cst_regex * const cst_rx_digits;
extern const cst_regex * const cst_rx_dotted_abbrev;

/* Table of regexps used in CART trees (only one so far) */
extern const cst_regex * const cst_regex_table[];
#define CST_RX_dotted_abbrev_NUM 0

#endif
