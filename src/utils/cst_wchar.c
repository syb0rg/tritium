#include "cst_wchar.h"
#include "cst_alloc.h"
#include "cst_string.h"

wchar_t *cst_cstr2wstr(const char *s)
{
    /* Actually this is naive and is really only for ASCII wchar_ts */
    int i,l;
    wchar_t *w;
    
    l=cst_strlen(s);
    w = cst_alloc(wchar_t,l+1);

    for (i=0; i<l; i++)
    {
        w[i] = (wchar_t)s[i];
    }
    w[i]=(wchar_t)'\0';
    
    return w;
}

char *cst_wstr2cstr(const wchar_t *w)
{
    int i,l;
    char *s;

    l=cst_wstrlen(w);
    s = cst_alloc(char,l+1);
    
    for (i=0; i<l; i++)
    {
        s[i] = (char)w[i];
    }
    s[i]=(wchar_t)'\0';
    
    return s;
}

