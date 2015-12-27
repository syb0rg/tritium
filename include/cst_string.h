#ifndef __CST_STRING_H__
#define __CST_STRING_H__

#include <string.h>

#if defined(UNDER_CE) && (UNDER_CE < 300)
#define isalnum(a) iswalnum((wint_t)(a))
#define isupper(a) iswupper((wint_t)(a))
#define islower(a) iswlower((wint_t)(a))
#endif

/* typedef unsigned char cst_string; */
typedef char cst_string;

double cst_atof(const char *str);

cst_string *cst_strdup(const cst_string *s);
cst_string *cst_strchr(const cst_string *s, int c);
cst_string *cst_strrchr(const cst_string *str, int c);
#define cst_strstr(h,n) \
     ((cst_string *)strstr((const char *)h,(const char *)n))
#define cst_strlen(s) (strlen((const char *)s))
#define streq(A,B) (strcmp(A,B) == 0)
#define streqn(A,B,N) (strncmp(A,B,N) == 0)
int cst_member_string(const char *str, const char * const *slist);
char *cst_substr(const char *str,int start, int length);

char *cst_string_before(const char *s,const char *c);

cst_string *cst_downcase(const cst_string *str);
cst_string *cst_upcase(const cst_string *str);

#endif
