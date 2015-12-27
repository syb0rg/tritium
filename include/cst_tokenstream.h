#ifndef _CST_TOKENSTREAM_H__
#define _CST_TOKENSTREAM_H__

#include "cst_alloc.h"
#include "cst_string.h"
#include "cst_file.h"

typedef struct  cst_tokenstream_struct {
    cst_file fd;
    int file_pos;
    int line_number;
    cst_string *string_buffer;

    int current_char;

    int token_pos;
    int ws_max;
    cst_string *whitespace;
    int prep_max;
    cst_string *prepunctuation;
    int token_max;
    cst_string *token;
    int postp_max;
    cst_string *postpunctuation;

    /* Should only be set through set_charclasses as charclass table needs */
    /* to be updated when you reset these                                  */
    const cst_string *p_whitespacesymbols;
    const cst_string *p_singlecharsymbols;
    const cst_string *p_prepunctuationsymbols;
    const cst_string *p_postpunctuationsymbols;

    cst_string charclass[256];
} cst_tokenstream;

#define TS_CHARCLASS_NONE        0
#define TS_CHARCLASS_WHITESPACE  2
#define TS_CHARCLASS_SINGLECHAR  4
#define TS_CHARCLASS_PREPUNCT    8
#define TS_CHARCLASS_POSTPUNCT  16
#define TS_CHARCLASS_QUOTE      32

#define ts_charclass(C,CLASS,TS) ((TS)->charclass[(unsigned char)C] & CLASS)

extern const cst_string * const cst_ts_default_whitespacesymbols;
extern const cst_string * const cst_ts_default_prepunctuationsymbols;
extern const cst_string * const cst_ts_default_postpunctuationsymbols;
extern const cst_string * const cst_ts_default_singlecharsymbols;

/* Public functions for tokenstream manipulation */
cst_tokenstream *ts_open(const char *filename,
			 const cst_string *whitespacesymbols,
			 const cst_string *singlecharsymbols,
			 const cst_string *prepunctsymbols,
			 const cst_string *postpunctsymbols);
cst_tokenstream *ts_open_string(const cst_string *string,
				const cst_string *whitespacesymbols,
				const cst_string *singlecharsymbols,
				const cst_string *prepunctsymbols,
				const cst_string *postpunctsymbols);
void ts_close(cst_tokenstream *ts);

int ts_eof(cst_tokenstream *ts);
const cst_string *ts_get(cst_tokenstream *ts);

const cst_string *ts_get_quoted_token(cst_tokenstream *ts,
				char quote,
				char escape);

void set_charclasses(cst_tokenstream *ts,
		     const cst_string *whitespace,
		     const cst_string *singlecharsymbols,
		     const cst_string *prepunctuation,
		     const cst_string *postpunctuation);

int ts_read(void *buff, int size, int num, cst_tokenstream *ts);

int ts_set_stream_pos(cst_tokenstream *ts,int pos);
int ts_get_stream_pos(cst_tokenstream *ts);

#endif
