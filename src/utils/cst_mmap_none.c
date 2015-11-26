#include "cst_file.h"
#include "cst_error.h"
#include "cst_alloc.h"

cst_filemap *cst_mmap_file(const char *path)
{
    cst_dbgmsg("cst_mmap_file: unsupported on this platform");
    cst_error();

    return NULL;
}

int cst_munmap_file(cst_filemap *fmap)
{
    cst_dbgmsg("cst_munmap_file: unsupported on this platform");
    cst_error();

    return -1;
}

cst_filemap *cst_read_whole_file(const char *path)
{
    cst_filemap *fmap;
    cst_file fh;

    if ((fh = cst_fopen(path, CST_OPEN_READ)) < 0) {
	cst_errmsg("cst_read_whole_file: Failed to open file\n");
	return NULL;
    }

    fmap = cst_alloc(cst_filemap, 1);
    fmap->fh = fh;
    fmap->mapsize = cst_filesize(fmap->fh);
    fmap->mem = cst_alloc(char, fmap->mapsize);
    if (cst_fread(fmap->fh, fmap->mem, 1, fmap->mapsize) < fmap->mapsize)
    {
	cst_errmsg("cst_read_whole_file: read() failed\n");
	cst_fclose(fmap->fh);
	cst_free(fmap->mem);
	cst_free(fmap);
	return NULL;
    }

    return fmap;
}

int cst_free_whole_file(cst_filemap *fmap)
{
    if (cst_fclose(fmap->fh) < 0) {
	cst_errmsg("cst_free_whole_file: close() failed\n");
	return -1;
    }
    cst_free(fmap->mem);
    cst_free(fmap);
    return 0;
}

cst_filemap *cst_read_part_file(const char *path)
{
    cst_filemap *fmap;
    cst_file fh;

    if ((fh = cst_fopen(path, CST_OPEN_READ)) == NULL) {
	cst_errmsg("cst_read_part_file: Failed to open file\n");
	return NULL;
    }

    fmap = cst_alloc(cst_filemap, 1);
    fmap->fh = fh;
    fmap->mapsize = cst_filesize(fmap->fh);

    return fmap;
}

int cst_free_part_file(cst_filemap *fmap)
{
    if (cst_fclose(fmap->fh) < 0) {
	cst_errmsg("cst_munmap_file: cst_fclose() failed\n");
	return -1;
    }
    cst_free(fmap);
    return 0;
}
