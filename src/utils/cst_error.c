#include <stdlib.h>
#include <stdarg.h>
#include "cst_file.h"
#include "cst_error.h"

#ifdef UNDER_CE

#include <winbase.h>
#include <stdlib.h>
#include "cst_alloc.h"

int cst_errmsg(const char *fmt, ...)
{
#ifdef DEBUG
    wchar_t wmsg[256];
    wchar_t *wfmt;
    size_t len;
    va_list args;

    len = mbstowcs(NULL,fmt,0) + 1;
    wfmt = cst_alloc(wchar_t,len);
    mbstowcs(wfmt,fmt,len);

    va_start(args,fmt);
    _vsnwprintf(wmsg,256,wfmt,args);
    va_end(args);

    wmsg[255]=L'\0';
    cst_free(wfmt);
    MessageBoxW(0,wmsg,L"Error",0);
#endif
    return 0;
}
jmp_buf *cst_errjmp = 0;

#elif defined(__palmos__)
#ifdef __ARM_ARCH_4T__
/* We only support throwing errors in ARM land */
jmp_buf *cst_errjmp = 0;
#endif

char cst_error_msg[600];
int cst_errmsg(const char *fmt, ...)
{
    va_list args;
    int count;

    va_start(args,fmt);
    count = cst_vsprintf(cst_error_msg,fmt,args);
    va_end(args);
    return 0;
}
#else

jmp_buf *cst_errjmp = 0;

int cst_errmsg(const char *fmt, ...)
{
    va_list args;
    int rv;

    va_start(args, fmt);
    rv = vfprintf(stderr, fmt, args);
    va_end(args);

    return rv;
}

#endif
