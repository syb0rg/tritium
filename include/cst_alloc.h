#ifndef __CST_ALLOC_H__
#define __CST_ALLOC_H__

#ifndef TRUE
#define TRUE (1==1)
#endif
#ifndef FALSE
#define FALSE (1==0)
#endif

/* Global allocation (the only kind on Unix) */
void *cst_safe_alloc(int size);
void *cst_safe_calloc(int size);
void *cst_safe_realloc(void *p,int size);

/* Allocate on local heap (needed on WinCE for various reasons) */
#ifdef UNDER_CE
#include <windows.h>
typedef HANDLE cst_alloc_context;

cst_alloc_context new_alloc_context(int size);
void delete_alloc_context(cst_alloc_context ctx);

void *cst_local_alloc(cst_alloc_context ctx, int size);
void cst_local_free(cst_alloc_context ctx, void *p);
#else /* not UNDER_CE */
typedef void * cst_alloc_context;
#define new_alloc_context(size)   (NULL)
#define delete_alloc_context(ctx)
#define cst_local_alloc(ctx,size) cst_safe_alloc(size)
#define cst_local_free(cst,p)     cst_free(p)
#endif /* UNDER_CE */

/* The public interface to the alloc functions */

/* Note the underlying call is calloc, so everything is zero'd */
#define cst_alloc(TYPE,SIZE) ((TYPE *)cst_safe_alloc(sizeof(TYPE)*(SIZE)))
#define cst_calloc(TYPE,SIZE) ((TYPE *)cst_safe_calloc(sizeof(TYPE)*(SIZE)))
#define cst_realloc(P,TYPE,SIZE) ((TYPE *)cst_safe_realloc((void *)(P),sizeof(TYPE)*(SIZE)))

void cst_free(void *p);

#endif
