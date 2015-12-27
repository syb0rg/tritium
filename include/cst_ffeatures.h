#ifndef _CST_FFEATURES_H
#define _CST_FFEATURES_H

#include "cst_val.h"
#include "cst_item.h"

const cst_val *ph_vc(const cst_item *p);
const cst_val *ph_vlng(const cst_item *p);
const cst_val *ph_vheight(const cst_item *p);
const cst_val *ph_vrnd(const cst_item *p);
const cst_val *ph_vfront(const cst_item *p);
const cst_val *ph_ctype(const cst_item *p);
const cst_val *ph_cplace(const cst_item *p);
const cst_val *ph_cvox(const cst_item *p);

const cst_val *cg_duration(const cst_item *p);
const cst_val *cg_state_pos(const cst_item *p);
const cst_val *cg_state_place(const cst_item *p);
const cst_val *cg_state_index(const cst_item *p);
const cst_val *cg_state_rindex(const cst_item *p);
const cst_val *cg_phone_place(const cst_item *p);
const cst_val *cg_phone_index(const cst_item *p);
const cst_val *cg_phone_rindex(const cst_item *p);

void basic_ff_register(cst_features *ffunctions);

#endif /* _CST_FFEATURES_H */
