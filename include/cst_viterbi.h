#ifndef _CST_VITERBI_H__
#define _CST_VITERBI_H__

#include "cst_file.h"
#include "cst_math.h"
#include "cst_utterance.h"

typedef struct cst_vit_cand_struct {
    int score;
    cst_val *val;
    int ival, pos;
    cst_item *item;
    struct cst_vit_cand_struct *next;
} cst_vit_cand;
cst_vit_cand *new_vit_cand();
void vit_cand_set(cst_vit_cand *vc, cst_val *val);
void vit_cand_set_int(cst_vit_cand *vc, int ival);
void delete_vit_cand(cst_vit_cand *vc);

typedef struct cst_vit_path_struct {
    int score;
    int state;
    cst_vit_cand *cand;
    cst_features *f;
    struct cst_vit_path_struct *from;
    struct cst_vit_path_struct *next;
} cst_vit_path;
cst_vit_path *new_vit_path();
void delete_vit_path(cst_vit_path *vp);

typedef struct cst_vit_point_struct {
    cst_item *item;
    int num_states;
    int num_paths;
    cst_vit_cand *cands;
    cst_vit_path *paths;
    cst_vit_path **state_paths;
    struct cst_vit_point_struct *next;
} cst_vit_point;
cst_vit_point *new_vit_point();
void delete_vit_point(cst_vit_point *vp);

struct cst_viterbi_struct;

/* Functions for user call back, to find candiates at a point, and
   to join (and score) paths */
typedef cst_vit_cand *(cst_vit_cand_f_t)(cst_item *s,
					  struct cst_viterbi_struct *vd);
typedef cst_vit_path *(cst_vit_path_f_t)(cst_vit_path *p,
					  cst_vit_cand *c,
					  struct cst_viterbi_struct *vd);

typedef struct cst_viterbi_struct {
    int num_states;
    cst_vit_cand_f_t *cand_func;
    cst_vit_path_f_t *path_func;
    int big_is_good;

    cst_vit_point *timeline;
    cst_vit_point *last_point;
    cst_features *f;
} cst_viterbi;


cst_viterbi *new_viterbi(cst_vit_cand_f_t *cand_func, 
			 cst_vit_path_f_t *path_func);
void delete_viterbi(cst_viterbi *vd);

void viterbi_initialise(cst_viterbi *vd,cst_relation *r);
void viterbi_decode(cst_viterbi *vd);
int viterbi_result(cst_viterbi *vd, const char *n);
void viterbi_copy_feature(cst_viterbi *vd,const char *featname);

#endif
