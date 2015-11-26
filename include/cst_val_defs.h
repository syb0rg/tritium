#ifndef _CST_VAL_DEFS_H__
#define _CST_VAL_DEFS_H__

#include <stdlib.h>

/* Macro for defining new user structs as vals  */
#define CST_VAL_USER_TYPE_DCLS(NAME,TYPE)              \
extern const int cst_val_type_##NAME;                  \
TYPE *val_##NAME(const cst_val *v);           \
cst_val *NAME##_val(const TYPE *v);

#define CST_VAL_USER_FUNCPTR_DCLS(NAME,TYPE)           \
extern const int cst_val_type_##NAME;                  \
TYPE val_##NAME(const cst_val *v);            \
cst_val *NAME##_val(const TYPE v);

#define CST_VAL_REGISTER_TYPE(NAME,TYPE)               \
TYPE *val_##NAME(const cst_val *v)                     \
{                                                      \
    return (TYPE *)val_generic(v,cst_val_type_##NAME,#NAME);  \
}                                                      \
void val_delete_##NAME(void *v)                        \
{                                                      \
    delete_##NAME((TYPE *)v);                          \
}                                                      \
                                                       \
cst_val *NAME##_val(const TYPE *v)                     \
{                                                      \
    return val_new_typed(cst_val_type_##NAME,          \
 		         (void *)v);                   \
}                                                      \

#define CST_VAL_REG_TD_TYPE(NAME,TYPE,NUM)             \
extern const int cst_val_type_##NAME;                  \
const int cst_val_type_##NAME=NUM;                     \
void val_delete_##NAME(void *v);                       \

/* When objects of this type can never be owned by vals */
#define CST_VAL_REGISTER_TYPE_NODEL(NAME,TYPE)         \
TYPE *val_##NAME(const cst_val *v)                     \
{                                                      \
    return (TYPE *)val_generic(v,cst_val_type_##NAME,#NAME);  \
}                                                      \
                                                       \
cst_val *NAME##_val(const TYPE *v)                     \
{                                                      \
    return val_new_typed(cst_val_type_##NAME,          \
 		         (void *)v);                   \
}                                                      \

#define CST_VAL_REG_TD_TYPE_NODEL(NAME,TYPE,NUM)       \
extern const int cst_val_type_##NAME;                  \
const int cst_val_type_##NAME=NUM;                     \
void val_delete_##NAME(void *v) { (void)v; }           \

#define CST_VAL_REGISTER_FUNCPTR(NAME,TYPE)            \
TYPE val_##NAME(const cst_val *v)                      \
{                                                      \
    return (TYPE)val_generic(v,cst_val_type_##NAME,#NAME);  \
}                                                      \
                                                       \
cst_val *NAME##_val(const TYPE v)                      \
{                                                      \
    return val_new_typed(cst_val_type_##NAME,          \
 		         (void *)v);                   \
}                                                      \

#define CST_VAL_REG_TD_FUNCPTR(NAME,TYPE,NUM)          \
extern const int cst_val_type_##NAME;                  \
const int cst_val_type_##NAME=NUM;                     \
void val_delete_##NAME(void *v) { (void)v; }           \

#endif
