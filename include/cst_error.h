#ifndef _CST_ERROR_H__
#define _CST_ERROR_H__

#include <stdlib.h>

#ifdef DIE_ON_ERROR
# ifdef UNDER_CE
#  define cst_error() *(int *)0=0
# else
#  define cst_error() abort()
# endif
#elif __palmos__
#ifdef __ARM_ARCH_4T__

typedef long *jmp_buf[10]; /* V1-V8, SP, LR (see po_setjmp.c) */
extern jmp_buf *cst_errjmp;
extern char cst_error_msg[];
int setjmp(register jmp_buf env);
void longjmp(register jmp_buf env, register int value);

# define cst_error() (cst_errjmp ? longjmp(*cst_errjmp,1) : 0)
#else  /* m68K */
/* I've never tested this or even compiled it (Tritium is ARM compiled) */
#  define cst_error() ErrFatalDisplayIf(-1, "cst_error")
#endif
#else /* not palmos */
#include <setjmp.h>
extern jmp_buf *cst_errjmp;
# define cst_error() (cst_errjmp ? longjmp(*cst_errjmp,1) : exit(-1))
#endif

/* WinCE sometimes doesn't have stdio, so this is a wrapper for
   fprintf(stderr, ...) */
int cst_errmsg(const char *fmt, ...);
#define cst_dbgmsg cst_errmsg

/* Need macros to help set catches */

#endif
