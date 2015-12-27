#ifndef _CST_SS_H__
#define _CST_SS_H__

typedef struct cst_ss_struct {
    double num_samples;
    double sum;
    double sumx;
} cst_ss;

cst_ss *new_ss();
void delete_ss(cst_ss *ss);
void ss_reset(cst_ss *ss);
double ss_mean(cst_ss *ss);
double ss_variance(cst_ss *ss);
double ss_stddev(cst_ss *ss);
void ss_cummulate(cst_ss *ss,double a);
void ss_cummulate_n(cst_ss *ss,double a, double count);

#endif
