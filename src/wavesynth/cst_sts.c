#include "cst_math.h"
#include "cst_hrg.h"
#include "cst_wave.h"
#include "cst_sigpr.h"
#include "cst_sts.h"

CST_VAL_REGISTER_TYPE_NODEL(sts_list,cst_sts_list)

cst_sts_list *new_sts_list()
{
    cst_sts_list *l = cst_alloc(struct cst_sts_list_struct,1);
    return l;
}

void delete_sts_list(cst_sts_list *l)
{
    if (l)
    {
	/* sub data is always const so can't free it */
	cst_free(l);
    }
    return;
}

int get_unit_size(const cst_sts_list *s,int start, int end)
{
    /* returns size (in samples) of unit */
    int i,size;

    for (i=start,size=0; i<end; i++)
	size += get_frame_size(s, i);

    return size;
}

int get_frame_size(const cst_sts_list *sts_list, int frame)
{
    if (sts_list->sts) 
	return sts_list->sts[frame].size;
    else if (sts_list->sts_paged)
	return sts_list->sts_paged[frame].res_size;
    else 
    {
	/* This assumes that the voice compiler has generated an extra
           offset at the end of the array. */
	return sts_list->resoffs[frame+1] - sts_list->resoffs[frame];
    } 
}

const unsigned short * get_sts_frame(const cst_sts_list *sts_list, int frame)
{
    if (sts_list->sts)
        return sts_list->sts[frame].frame;
    else if (sts_list->sts_paged)
        return &sts_list->sts_paged[frame].frame_page[sts_list->num_channels * sts_list->sts_paged[frame].frame_offset];
    else
        return sts_list->frames + (frame * sts_list->num_channels);
}

const unsigned char * get_sts_residual(const cst_sts_list *sts_list, int frame)
{
    if (sts_list->sts)
        return sts_list->sts[frame].residual;
    else if (sts_list->sts_paged)
        return &sts_list->sts_paged[frame].res_page[sts_list->sts_paged[frame].res_offset];
    else 
        return sts_list->residuals + sts_list->resoffs[frame];
}

const unsigned char *get_sts_residual_fixed(const cst_sts_list *sts_list, int frame)
{
    /* Actually for mceps */
    if (sts_list->sts)
	return sts_list->sts[frame].residual;
    else if (sts_list->sts_paged)
        return &sts_list->sts_paged[frame].res_page[sts_list->sts_paged[frame].res_offset];
    else
	return 
            (const unsigned char *)sts_list->residuals
	    + (frame * sts_list->num_channels);

}

