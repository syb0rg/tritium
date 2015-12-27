#ifndef __CST_ENDIAN_H__
#define __CST_ENDIAN_H__

extern const int cst_endian_loc;
/* Sun, HP, SGI Mips, M68000 */
#define CST_BIG_ENDIAN (((char *)&cst_endian_loc)[0] == 0)
/* Intel, Alpha, DEC Mips, Vax, Arm, Other MIPS (Casio etc) */
#define CST_LITTLE_ENDIAN (((char *)&cst_endian_loc)[0] != 0)

/* EST byte order strings */
#define BYTE_ORDER_BIG "10"
#define BYTE_ORDER_LITTLE "01"

#define SWAPINT(x) ((((unsigned int)x) & 0xff) << 24 | \
        (((unsigned int)x) & 0xff00) << 8 | \
	(((unsigned int)x) & 0xff0000) >> 8 | \
        (((unsigned int)x) & 0xff000000) >> 24)
/* For m68k we want to be a little more explicit */
#define SWAPLONG(x) ((((unsigned long)x) & 0xff) << 24 | \
        (((unsigned long)x) & 0xff00) << 8 | \
	(((unsigned long)x) & 0xff0000) >> 8 | \
        (((unsigned long)x) & 0xff000000) >> 24)
#define SWAPSHORT(x) ((((unsigned short)x) & 0xff) << 8 | \
        (((unsigned short)x) & 0xff00) >> 8)

void swap_bytes_short(short * b,int n);

void swapdouble(double *d);
void swapfloat(float *f);

#endif
