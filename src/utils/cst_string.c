#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cst_alloc.h"
#include "cst_string.h"

#ifdef UNDER_CE /* WinCE does not fully implement ANSI C */

cst_string *cst_strrchr(const cst_string *str, int c)
{
    cst_string *p = (const cst_string *)str + cst_strlen(str);
    while (p >= str) {
	if (*p == c)
	    return p;
	--p;
    }
    return NULL;
}

double cst_atof(const char *str)
{
    /* double f = 0.0; */
    
    /* sscanf(str, "%f", &f); */
    return atof(str);
}

#else /* Sane operating system */

cst_string *cst_strrchr(const cst_string *str, int c)
{
    return (cst_string *)strrchr((const char *)str, c);
}

double cst_atof(const char *str)
{
    return atof(str);
}
#endif /* WinCE */

cst_string *cst_strdup(const cst_string *str)
{
    cst_string *nstr = NULL;

    if (str)
    {
	nstr = cst_alloc(cst_string,cst_strlen((const char *)str)+1);
	memmove(nstr,str,cst_strlen((const char *)str)+1);
    }
    return nstr;
}

cst_string *cst_strchr(const cst_string *s, int c)
{
    return (cst_string *)strchr((const char *)s,c);
}

char *cst_substr(const char *str,int start, int length)
{
    char *nstr = NULL;

    if (str)
    {
	nstr = cst_alloc(char,length+1);
	strncpy(nstr,str+start,length);
	nstr[length] = '\0';
    }
    return nstr;
}

char *cst_string_before(const char *s,const char *c)
{
    char *p;
    char *q;

    p = (char *)cst_strstr(s,c);
    if (p == NULL) 
	return NULL;
    q = (char *)cst_strdup((cst_string *)s);
    q[cst_strlen(s)-cst_strlen(p)] = '\0';
    return q;
}

cst_string *cst_downcase(const cst_string *str)
{
    cst_string *dc;
    int i;

    dc = cst_strdup(str);
    for (i=0; str[i] != '\0'; i++)
    {
	if (isupper((int)str[i]))
	    dc[i] = tolower((int)str[i]);
    }
    return dc;
}

cst_string *cst_upcase(const cst_string *str)
{
    cst_string *uc;
    int i;

    uc = cst_strdup(str);
    for (i=0; str[i] != '\0'; i++)
    {
	if (islower((int)str[i]))
	    uc[i] = toupper((int)str[i]);
    }
    return uc;
}

int cst_member_string(const char *str, const char * const *slist)
{
    const char * const *p;

    for (p = slist; *p; ++p)
	if (streq(*p, str))
	    break;

    return *p != NULL;
}
