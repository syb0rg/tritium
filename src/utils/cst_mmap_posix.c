#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "cst_file.h"
#include "cst_error.h"
#include "cst_alloc.h"

#ifdef __QNXNTO__
#include <sys/syspage.h>
#define getpagesize() (SYSPAGE_ENTRY( system_private )->pagesize)
#else
#define MAP_NOSYNCFILE 0
#endif

cst_filemap *cst_mmap_file(const char *path)
{
    cst_filemap *fmap = NULL;
    size_t pgsize;
    struct stat buf;
    int fd;

    pgsize = getpagesize();

    if ((fd = open(path, O_RDONLY)) < 0) {
	perror("cst_mmap_file: Failed to open file");
	return NULL;
    }
    if ((fstat(fd, &buf)) < 0) {
	perror("cst_mmap_file: fstat() failed");
	return NULL;
    }
    fmap = cst_alloc(cst_filemap, 1);
    fmap->fd = fd;
    fmap->mapsize = (buf.st_size + pgsize - 1) / pgsize * pgsize;
    if ((fmap->mem = mmap(0, fmap->mapsize, PROT_READ,
			  MAP_SHARED | MAP_NOSYNCFILE, fd, 0)) == (caddr_t)-1) {
	perror("cst_mmap_file: mmap() failed");
	cst_free(fmap);
	return NULL;
    }

    return fmap;
}

int cst_munmap_file(cst_filemap *fmap)
{
    if (munmap(fmap->mem, fmap->mapsize) < 0) {
	perror("cst_munmap_file: munmap() failed");
	return -1;
    }
    if (close(fmap->fd) < 0) {
	perror("cst_munmap_file: close() failed");
	return -1;
    }
    cst_free(fmap);
    return 0;
}

cst_filemap *cst_read_whole_file(const char *path)
{
    cst_filemap *fmap;
    struct stat buf;
    int fd;

    if ((fd = open(path, O_RDONLY)) < 0) {
	perror("cst_read_whole_file: Failed to open file");
	return NULL;
    }
    if ((fstat(fd, &buf)) < 0) {
	perror("cst_read_whole_file: fstat() failed");
	return NULL;
    }

    fmap = cst_alloc(cst_filemap, 1);
    fmap->fd = fd;
    fmap->mapsize = buf.st_size;
    fmap->mem = cst_alloc(char, fmap->mapsize);
    if (read(fmap->fd, fmap->mem, fmap->mapsize) < fmap->mapsize)
    {
	perror("cst_read_whole_fiel: read() failed");
	close(fmap->fd);
	cst_free(fmap->mem);
	cst_free(fmap);
	return NULL;
    }

    return fmap;
}

int cst_free_whole_file(cst_filemap *fmap)
{
    if (close(fmap->fd) < 0) {
	perror("cst_free_whole_file: close() failed");
	return -1;
    }
    cst_free(fmap->mem);
    cst_free(fmap);
    return 0;
}

cst_filemap *cst_read_part_file(const char *path)
{
    cst_filemap *fmap;
    struct stat buf;
    cst_file fh;

    if ((fh = cst_fopen(path, CST_OPEN_READ)) == NULL) {
	perror("cst_read_part_file: Failed to open file");
	return NULL;
    }
    if ((fstat(fileno(fh), &buf)) < 0) {
	perror("cst_read_part_file: fstat() failed");
	return NULL;
    }

    fmap = cst_alloc(cst_filemap, 1);
    fmap->fh = fh;
    fmap->mapsize = buf.st_size;

    return fmap;
}

int cst_free_part_file(cst_filemap *fmap)
{
    if (cst_fclose(fmap->fh) < 0) {
	perror("cst_munmap_file: cst_fclose() failed");
	return -1;
    }
    cst_free(fmap);
    return 0;
}
