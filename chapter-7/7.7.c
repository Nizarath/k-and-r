/* grep without regexs, features:
 * - supports both stdin/file arg modes;
 * - options are (must be before arguments)
 *   -i
 *   -v
 *   -c
 *   -n
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* I/O */

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

int fputln(char *s, FILE *fp)
{
	for (; *s; s++)
		putc(*s, fp);
	return ferror(fp);
}

/* grep CLI options */

static int v = 0;		/* except */
static int c = 0;		/* only count */
static int n = 0;		/* linenos */
/* ignoring the case */
extern char *strcasestr(const char *, const char *);
static char *(*find)(const char *, const char *) = strstr;
static int multiple = 0;	/* > 1 file to read, need prefix name in output */

/* grep output */
static int lineno, cnt;		/* line num, match cnt */

void grep(const char *pattern, const char *fname)
{
	FILE *fp;
	char s[MAXLINE];

	if (fname == NULL)		/* open a file part */
		fp = stdin;
	else {
		fp = fopen(fname, "r");
		if (fp == NULL) {
			fprintf(stderr, "grep: error opening %s\n", fname);
			return;
		}
	}

	/* main loop */
	for (lineno = 1, cnt = 0; fgetln(s, MAXLINE, fp) > 0; lineno++)
		if (((*find)(s, pattern) != NULL) != v) {
			cnt++;
			if (c)
				continue;
			if (multiple)
				printf("%s:", fname);
			if (n)
				printf("%d:", lineno);
			fputln(s, stdout);
		}
	fclose(fp);
	if (c) {
		if (multiple)
			printf("%s:", fname);
		printf("%d\n", cnt);
	}
}

int main(int argc, char *argv[])
{
	char *pattern;

	while (--argc && **++argv == '-')
		while (*++*argv)
			switch (**argv) {
				case 'c': c = 1; break;
				case 'i': find = strcasestr; break;
				case 'v': v = 1; break;
				case 'n': n = 1; break;
				case 'h':
					  printf("usage: grep [-v] [-i] [-c] [-n]"
						" <pattern> [file 1] [file 2] [...]"
						" [file n]\n");
					  exit(0);
				default:
					  fprintf(stderr, "grep: unknown option: %c\n"
							  "-h for help\n",
							  **argv);
					  exit(2);
			}
	if (!argc) {
		fprintf(stderr, "grep: missed pattern\n-h for help\n");
		exit(2);
	}
	if (argc <= 1)
		grep(*argv, NULL);		/* main work */
	else {
		if (argc > 2)
			multiple = 1;
		for (pattern = *argv++; --argc; argv++)
			grep(pattern, *argv);	/* main work */
	}

	exit(0);
}
