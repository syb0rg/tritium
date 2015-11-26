#include "cst_alloc.h"
#include "cst_ss.h"
#include "cst_math.h"

cst_ss *new_ss()
{
    cst_ss *ss = cst_alloc(cst_ss,1);
    return ss;
}

void delete_ss(cst_ss *ss)
{
    cst_free(ss);
}

void ss_reset(cst_ss *ss)
{
    ss->num_samples = 0;
    ss->sum = 0;
    ss->sumx = 0;
}

double ss_mean(cst_ss *ss)
{
    if (ss->num_samples > 0)
	return ss->sum/ss->num_samples;
    else
	return 0;
}

double ss_variance(cst_ss *ss)
{
    if (ss->num_samples > 1)
	return ((ss->num_samples*ss->sumx)-(ss->sum*ss->sum))/
	    (ss->num_samples*(ss->num_samples-1));
    else
	return 0;
}

double ss_stddev(cst_ss *ss)
{
    return sqrt(ss_variance(ss));
}

void ss_cummulate(cst_ss *ss,double a)
{
    ss->sum += a;
    ss->sumx += a*a;
    ss->num_samples++;
}

void ss_cummulate_n(cst_ss *ss,double a, double count)
{
    ss->sum += a*count;
    ss->sumx += (a*a)*count;
    ss->num_samples += count;
}

