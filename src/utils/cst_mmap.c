#ifdef _WIN32

#include <windows.h>

#ifndef MEM_TOP_DOWN
#define MEM_TOP_DOWN 0
#endif

#include "cst_file.h"
#include "cst_error.h"
#include "cst_alloc.h"

cst_filemap *cst_mmap_file(const char *path)
{
	HANDLE ffm;
	cst_filemap *fmap = NULL;

	ffm = CreateFile(path,GENERIC_READ,FILE_SHARE_READ,NULL,
			 OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (ffm == INVALID_HANDLE_VALUE) {
		return NULL;
	} else {
		fmap = cst_alloc(cst_filemap,1);
		fmap->h = CreateFileMapping(ffm,NULL,PAGE_READONLY,0,0,NULL);
		fmap->mapsize = GetFileSize(fmap->h, NULL);
		fmap->mem = MapViewOfFile(fmap->h,FILE_MAP_READ,0,0,0);
		if (fmap->h == NULL || fmap->mem == NULL) {
			CloseHandle(ffm);
			cst_free(fmap);
			return NULL;
		}
	}

	return fmap;
}

int cst_munmap_file(cst_filemap *fmap)
{
	UnmapViewOfFile(fmap->mem);
	CloseHandle(fmap->h);
	cst_free(fmap);
	return 0;
}

cst_filemap *cst_read_whole_file(const char *path)
{
    cst_filemap *fmap;
    cst_file fh;

    if ((fh = cst_fopen(path, CST_OPEN_READ)) == NULL) {
	cst_errmsg("cst_read_whole_file: Failed to open file\n");
	return NULL;
    }

    fmap = cst_alloc(cst_filemap, 1);
    fmap->fh = fh;
    cst_fseek(fmap->fh, 0, CST_SEEK_ENDREL);
    fmap->mapsize = cst_ftell(fmap->fh);
    fmap->mem = VirtualAlloc(NULL, fmap->mapsize, MEM_COMMIT|MEM_TOP_DOWN,
			     PAGE_READWRITE);
    cst_fseek(fmap->fh, 0, CST_SEEK_ABSOLUTE);
    cst_fread(fmap->fh, fmap->mem, 1, fmap->mapsize);

    return fmap;
}

int cst_free_whole_file(cst_filemap *fmap)
{
    if (cst_fclose(fmap->fh) < 0) {
	cst_errmsg("cst_read_whole_file: cst_fclose() failed\n");
	return -1;
    }
    VirtualFree(fmap->mem, fmap->mapsize, MEM_DECOMMIT);
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

    return fmap;
}

int cst_free_part_file(cst_filemap *fmap)
{
    if (cst_fclose(fmap->fh) < 0) {
	cst_errmsg("cst_read_part_file: cst_fclose() failed\n");
	return -1;
    }
    cst_free(fmap);
    return 0;
}

#elif __unix__

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

#endif
