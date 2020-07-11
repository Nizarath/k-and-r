#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* normal string comparator */
int strcmp(const char *s0, const char *s1)
{
	for (; *s0 == *s1; s0++, s1++)
		if (*s0 == '\0')
			return 0;
	return *s0 - *s1;
}

/* ignore case */
int stricmp(const char *s0, const char *s1)
{
	char c0, c1;

	for (; *s0 == *s1; s0++, s1++)
		if (*s0 == '\0')
			return 0;
	c0 = *s0, c1 = *s1;
	if (c0 >= 'A' && c0 <= 'Z')
		c0 += 'a' - 'A';
	if (c1 >= 'A' && c1 <= 'Z')
		c1 += 'a' - 'A';
	return c0 - c1;
}

/* directory order */
int strdcmp(const char *s0, const char *s1)
{
	for (; *s0 == *s1; s0++, s1++)
		if (*s0 == '\0' || !isalnum(*s0))
			return 0;
	return *s0 - *s1;
}

/* directory order with case ignoring */
int strdicmp(const char *s0, const char *s1)
{
	char c0, c1;

	for (; *s0 == *s1; s0++, s1++)
		if (*s0 == '\0' || !isalnum(*s0))
			return 0;
	c0 = *s0, c1 = *s1;
	if (c0 >= 'A' && c0 <= 'Z')
		c0 += 'a' - 'A';
	if (c1 >= 'A' && c1 <= 'Z')
		c1 += 'a' - 'A';
	return c0 - c1;
}

/* getline */
#define MAXLINE		100

int getln(char *s, int max)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (max - 1) &&
			(c = getchar()) != EOF; *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';

	return s - b;
}

/* getlines */
#define MAXLINES	1000

int readlines(char *lines[], int maxlines)
{
	char buf[MAXLINE];
	int lineno;

	for (lineno = 0; getln(buf, MAXLINE) > 0; lineno++)
		/* too big count or alloc error */
		if (lineno >= maxlines || ((lines[lineno] = strdup(buf)) == NULL))
			return -1;

	return lineno;		/* line count */
}

/* qsort */
int scmp(const void *sp0, const void *sp1)
{
	return strcmp(*(const char **)sp0, *(const char **)sp1);
}

int scmp_rev(const void *sp0, const void *sp1)
{
	return strcmp(*(const char **)sp1, *(const char **)sp0);
}

int sicmp(const void *sp0, const void *sp1)
{
	return stricmp(*(const char **)sp0, *(const char **)sp1);
}

int sicmp_rev(const void *sp0, const void *sp1)
{
	return stricmp(*(const char **)sp1, *(const char **)sp0);
}

int sdcmp(const void *sp0, const void *sp1)
{
	return strdcmp(*(const char **)sp0, *(const char **)sp1);
}

int sdcmp_rev(const void *sp0, const void *sp1)
{
	return strdcmp(*(const char **)sp1, *(const char **)sp0);
}

int sdicmp(const void *sp0, const void *sp1)
{
	return strdicmp(*(const char **)sp0, *(const char **)sp1);
}

int sdicmp_rev(const void *sp0, const void *sp1)
{
	return strdicmp(*(const char **)sp1, *(const char **)sp0);
}

int ncmp(const void *np0, const void *np1)
{
	int i0, i1;

	i0 = atof(*(const char **)np0);
	i1 = atof(*(const char **)np1);

	return (i0 < i1) ? -1 : (i0 > i1) ? 1 : 0;
}

int ncmp_rev(const void *np0, const void *np1)
{
	int i0, i1;

	i0 = atof(*(const char **)np0);
	i1 = atof(*(const char **)np1);

	return (i1 < i0) ? -1 : (i1 > i0) ? 1 : 0;
}

/* print lines */
void prlines(char *lines[], int maxlines)
{
	while (maxlines--)
		fputs(*lines++, stdout);
}

int main(int argc, char *argv[])
{
	int r, n, f, d;		/* CLI */
	char *lines[MAXLINES];
	int linecnt;
	/* duplications to avoid any checking */
	int (*cmp[])(const void *, const void *) = {
		scmp, scmp_rev, sicmp, sicmp_rev, sdcmp, sdcmp_rev, sdicmp, sdicmp_rev,
		ncmp, ncmp_rev, ncmp, ncmp_rev, ncmp, ncmp_rev, ncmp, ncmp_rev
	};

	/* read CLI */
	for (d = f = r = n = 0; --argc && *++argv; )
		while (*++*argv)
			switch (**argv) {
				case 'n': n = 1; break;	/* numerical */
				case 'r': r = 1; break;	/* reverse */
				case 'f': f = 1; break;	/* ignore case */
				case 'd': d = 1; break;	/* directory order */
				default:
					  printf("unknown option: %c\n", **argv);
					  return 2;
			}

	/* read lines */
	linecnt = readlines(lines, MAXLINES);
	if (linecnt == -1) {
		fprintf(stderr, "input error\n");
		return 1;
	}

	/* sort them */
	qsort(lines, linecnt, sizeof(char *), cmp[n * 8 + d * 4 + f * 2 + r]);

	/* show */
	prlines(lines, linecnt);
	return 0;
}
