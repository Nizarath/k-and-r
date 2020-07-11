/*
 * cat based on the libc, version
 * with reading hole lines (meaningless, but to compare
 * with byte reader)
 */

#include <stdio.h>
#include <stdlib.h>

/* fgets/fputs */
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

int fputs(const char *s, FILE *fp)
{
	while (*s)
		putc(*s++, fp);
	return ferror(fp);
}

/* main work */
int main(int argc, char *argv[])
{
	FILE *fp;
	extern void cat(FILE *fp);

	if (argc == 1)
		cat(stdin);
	else
		while (--argc && *++argv) {
			fp = fopen(*argv, "r");
			if (fp == NULL) {
				fprintf(stderr, "error opening %s\n", *argv);
				exit(1);
			}
			cat(fp);
			fclose(fp);
		}
	exit(0);
}

void cat(FILE *fp)
{
	char s[MAXLINE];

	while (fgets(s, MAXLINE, fp) > 0)
		if (fputs(s, stdout)) {
			fprintf(stderr, "error while writing\n");
			return;
		}
}
