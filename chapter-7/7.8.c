#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* some routines */

char *str2upper(char *s)
{
	register char *t;
	
	for (t = s; *t; t++)
		*t = toupper(*t);

	return s;
}

/* I/O part */

#define MAXLINE		1000

int fgetln(char *s, int max, FILE *fp)
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

int fputln(const char *s, FILE *fp)
{
	for (; *s; s++)
		putc(*s, fp);
	return ferror(fp);
}

/* numeration of pages, running through all files */
static int pageno = 1;
static int lineno = 1;
static int pagesize = 25;

/* pcat */
void cat(const char *fname)
{
	FILE *fp;
	char s[MAXLINE];
	register int lineno;

	if (fname != NULL) {
		fp = fopen(fname, "r");
		if (fp == NULL) {
			fprintf(stderr, "pcat: error opening: %s\n", fname);
			return;
		}
	} else
		fp = stdin;
	if (fname != NULL)
		printf("============== %s ============\n",
				str2upper((char *)fname));
	while (fgetln(s, MAXLINE, fp) > 0) {
		if (++lineno > pagesize) {
			lineno = 1;
			printf("================= %d ===============\n", pageno++);
		}
		fputln(s, stdout);
	}
	fclose(fp);
}

int main(int argc, char *argv[])
{
	while (--argc && **++argv == '-') {
		while (*++*argv)
			switch (**argv) {
				case 'n':
					pagesize = atoi(*++argv);
					argc--;
					goto next;
				default:
					fprintf(stderr, "pcat: unknown option: %c\n",
							  **argv);
					exit(2);
			}
	next:	;
	}
	if (!argc)
		cat(NULL);
	else
		while (argc--)
			cat(*argv++);
	return 0;
}
