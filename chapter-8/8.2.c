/*
 * stdio.c implementation on UNIX syscalls
 */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define OPEN_MAX	20
#define EOF		(-1)
#undef NULL		/* stdlib.h */
#define NULL		((void *) 0)
#define BUFSIZ		(1 << 12)	/* better to be the divisor
					   of FS block */

struct flag {
	unsigned rd : 1;	/* read support */
	unsigned wr : 1;	/* write support */
	unsigned unbuf : 1;	/* buffersize = 1 */
	unsigned eof : 1;	/* end of file */
	unsigned err : 1;	/* error */
	unsigned : 0;		/* alignment by the next machine word */
};

typedef struct {
	/* buffer itself */
	char *buf;	/* base */
	int bp;		/* next char to read/write */
	int cnt;	/* cnt to read/write */
	
	int fd;		/* file descriptor */
	struct flag flag;	/* flags */
	int used;	/* is in use */
} FILE;

static FILE _fbuf[OPEN_MAX] = {
	{ NULL, 0, 0, 0, {1, 0, 0, 0, 0}, 1 },	/* stdin */
	{ NULL, 0, 0, 1, {0, 1, 0, 0, 0}, 1 },	/* stdout */
	{ NULL, 0, 0, 2, {0, 1, 1, 0, 0}, 1 } /* stderr */
};

#define stdin		(&_fbuf[0])
#define stdout		(&_fbuf[1])
#define stderr		(&_fbuf[2])

/* 
 * functions from libc (+ == done):
 *
 * open/close:
 + fopen -- FILE *fopen(const char *name, const char *mode);
 + fclose -- int fclose(FILE *fp);
 *
 * i/o:
 + getc -- int getc(FILE *fp);
 + putc -- int putc(int c, FILE *fp);
 + fgetc -- int fgetc(FILE *fp);
 + fputc -- int fputc(int c, FILE *fp);
 + getchar -- int getchar(void);
 + putchar -- int putchar(int c);
 + fgets -- char *fgets(char *s, int max, FILE *fp);
 + fputs -- int fputs(const char *s, FILE *fp);
 *
 * check:
 + feof -- int feof(FILE *fp);
 + ferror -- int ferror(FILE *fp);
 + fileno -- int fileno(FILE *fp);
 *
 * other:
 + fseek -- int fseek(FILE *fp, long off, int base);
 */

int feof(FILE *fp)
{
	return fp->flag.eof;
}

int ferror(FILE *fp)
{
	return fp->flag.err;
}

int fileno(FILE *fp)
{
	return fp->fd;
}

/*
 * supports following modes:
 * - "r" for read;
 * - "w" for write;
 * - "a" for append;
 * "b" modes are crap that exist on non-UNIX systems for binary
 * hint: it doesn't read the buffer, since file may never be read, only opened
 * for some reasons
 */
FILE *fopen(const char *name, const char *mode)
{
	register FILE *fp;
	int fd;
	struct flag tmp = { 0, 0, 0, 0, 0 };

	/* loop across table */
	for (fp = _fbuf; fp < &_fbuf[OPEN_MAX]; fp++)
		if (!fp->used)
			goto found;
	return NULL;		/* not found */

found:
	/* determine mode */
	switch (*mode) {
		case 'a':		/* append */
		case 'w':		/* replace */
			fd = open(name, O_WRONLY);
			fp->flag.wr = 1;
			break;
		case 'r':		/* read */
			fd = open(name, O_RDONLY);
			fp->flag.rd = 1;
			break;
		default:		/* unknown option, some crap */
			return NULL;
	}

	/* check correct open */
	if (fd < 0) {
		fp->flag = tmp;	/* don't need to set _ERR, it's not others fp */
		return NULL;
	}

	/* lseek if append */
	if (*mode == 'a')
		lseek(fd, 0L, 2);

	/* ok, now just finish */
	fp->fd = fd;
	fp->used = 1;	/* mark in use */
	fp->buf = NULL;	/* no buffer yet, see description */
	fp->bp = fp->cnt = 0;

	return fp;
}

int _fillbuf(FILE *fp)
{
	int bufsize;

	if (!fp->flag.rd) {	/* check rights */
		fp->flag.err = 1;
		return EOF;
	}

	/* determine buffer size */
	bufsize = fp->flag.unbuf ? 1 : BUFSIZ;

	if (fp->buf == NULL) {	/* need alloc */
		fp->buf = malloc(bufsize);
		if (fp->buf == NULL) {
			fp->flag.err = 1;
			return EOF;
		}
	}

	/* read buf */
	if ((fp->cnt = read(fp->fd, fp->buf, bufsize)) < 0) {	/* err */
		fp->flag.err = 1;
		return EOF;
	} else if (fp->cnt == 0) {		/* file finished */
		fp->flag.eof = 1;
		return EOF;
	}

	/* normal case */
	fp->bp = 1;
	fp->cnt--;

	return fp->buf[0];
}

int getc(FILE *fp)	/* avoid using macro for ++ cases */
{
	return fp->cnt-- ? fp->buf[fp->bp++] : _fillbuf(fp);
}

int fgetc(FILE *fp)	/* same shit */
{
	return getc(fp);
}

int putc(int c, FILE *fp)
{
	extern void _flushbuf(FILE *);

	if (fp->cnt <= 0)
		_flushbuf(fp);
	fp->cnt--;

	return fp->buf[fp->bp++] = c;
}

/* flush buffer to disk, write and return new symbol */
void _flushbuf(FILE *fp)
{
	int bufsize;

	if (!fp->flag.wr) {	/* check rights */
		fp->flag.err = 1;
		return;
	}

	/* determine size */
	bufsize = fp->flag.unbuf ? 1 : BUFSIZ;

	/* alloc buf if needed */
	if (fp->buf == NULL) {
		fp->buf = malloc(bufsize);
		if (fp->buf == NULL) {
			fp->flag.err = 1;
			return;
		}
	}

	/* write to disk */
	if (write(fp->fd, fp->buf, fp->bp) < 0) {
		fp->flag.err = 1;
		return;
	}

	/* update state */
	fp->cnt = bufsize;
	fp->bp = 0;
}

int fputc(int c, FILE *fp)
{
	return putc(c, fp);
}

int getchar(void)
{
	return getc(stdin);
}

int putchar(int c)
{
	return putc(c, stdout);
}

int fclose(FILE *fp)
{
	struct flag tmp = { 0, 0, 0, 0, 0};

	if (fp->flag.wr)	/* libc */
		_flushbuf(fp);
	fp->flag = tmp;
	fp->cnt = fp->bp = 0;
	free(fp->buf);
	fp->buf = NULL;
	fp->used = 0;
	return close(fp->fd);		/* unix */
}

int fflush(FILE *fp)
{
	if (!fp->flag.wr)
		return EOF;
	_flushbuf(fp);
}

/* gets/puts */

#define MAXLINE		100

int fgets(char *s, int max, FILE *fp)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (max - 1) &&
			(c = getc(fp)) != EOF; *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';

	return s - b;
}

int fputs(const char *s, FILE *fp)
{
	for (; *s; s++) {
		putc(*s, fp);
		if (*s == '\n')
			fflush(fp);
	}
	
	return 0;
}

/* cat part to test */

void cat(FILE *fp)
{
	char buf[MAXLINE];

	while (fgets(buf, MAXLINE, fp) > 0)
		fputs(buf, stdout);
}

/* fseek */
int fseek(FILE *fp, long off, int base)
{
	if (fp->flag.wr)
		fflush(fp);
	lseek(fp->fd, off, base);

	return 0;
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char *err = "can not open ";
	
	if (argc == 1)
		cat(stdin);
	else {
		while (--argc && *++argv) {
			fp = fopen(*argv, "r");
			if (fp == NULL) {
				fputs(err, stderr);
				fputs(*argv, stderr);
				putc('\n', stderr);
				putc('\n', stderr);
				continue;
			}
			cat(fp);
			fclose(fp);
		}
	}
	return 0;

}
