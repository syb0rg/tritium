#include "cst_file.h"
#include "cst_tokenstream.h"
#include "cst_item.h"
#include "cst_relation.h"
#include "cst_utterance.h"

int relation_load(cst_relation *r, const char *filename)
{
    cst_tokenstream *fd;
    cst_item *item;
    const char *token=0;

    if ((fd = ts_open(filename,NULL,";","","")) == 0)
    {
	cst_errmsg("relation_load: can't open file \"%s\" for reading\n",
		   filename);
	return CST_ERROR_FORMAT;
    }

    for ( ; !ts_eof(fd); )
    {
	token = ts_get(fd);
	if (streq("#",token))
	    break;
    }
    if (!streq("#",token))
    {
	cst_errmsg("relation_load: no end of header marker in \"%s\"\n",
		   filename);
	ts_close(fd);
	return CST_ERROR_FORMAT;
    }
	
    while (!ts_eof(fd))
    {
	token = ts_get(fd);
	if (streq(token,""))
	    continue;
	item = relation_append(r,NULL);
	item_set_float(item,"end",(float)cst_atof(token));
	token = ts_get(fd);
	token = ts_get(fd);
	item_set_string(item,"name",token);
    }

    ts_close(fd);
    return CST_OK_FORMAT;
}

int relation_save(cst_relation *r, const char *filename)
{
    cst_file fd;
    cst_item *item;

#ifndef UNDER_CE
    if (streq(filename,"-"))
	fd = stdout;
    else
#endif
	if ((fd = cst_fopen(filename,CST_OPEN_WRITE)) == 0)
    {
	cst_errmsg("relation_save: can't open file \"%s\" for writing\n",
		   filename);
	return CST_ERROR_FORMAT;
    }

    for (item=relation_head(r); item; item=item_next(item))
    {
        if (item_feat_present(item,"end"))
	    cst_fprintf(fd,"%f ",item_feat_float(item,"end"));
	else
	    cst_fprintf(fd,"%f ",0.00);
        if (item_feat_present(item,"name"))
	    cst_fprintf(fd,"%s ",item_feat_string(item,"name"));
	else
	    cst_fprintf(fd,"%s ","_");
	cst_fprintf(fd,"\n");
    }
#ifndef UNDER_CE
    if (fd != stdout)
	cst_fclose(fd);
#endif

    return CST_OK_FORMAT;
}

