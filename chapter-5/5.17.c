#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* getline part */
#define MAXLINE		100
int getln(char *s, int max)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (max - 1) &&
			(c = getchar()) != EOF && c != '\n'; *s++ = c)
		;
	*s = '\0';

	return s - b;
}

/* readlines */
#define MAXLINES	2000
int readlines(char *lines[], int maxlines)
{
	int lineno;
	char buf[MAXLINE];

	for (lineno = 0; getln(buf, maxlines) > 0; lineno++)
		if (lineno >= maxlines || (lines[lineno] = strdup(buf)) == NULL)
			return -1;
	return lineno;
}

void prlines(char *lines[], int linecnt)
{
	while (linecnt--)
		printf("%s\n", *lines++);
}

/* CLI options */
#define	OPT_N	(1 << 0)	/* numeric comparison */
#define	OPT_R	(1 << 1)	/* reverse comparison */
#define	OPT_F	(1 << 2)	/* ignore case while comparing */

/* line fields */
#define MAXFIELDS	5

int flds[MAXFIELDS];	/* each field has it's own comparison rule */
char sep = ' ';		/* separator, default val */

/* compare strings according to fields;
 * all is done in the lines themselves, no buffers */
int linecmp(const void *sp0, const void *sp1)
{
	register char *t0, *t1, *s0, *s1;
	register int fld, cmp;

	/* get concrete */
	s0 = *(char **)sp0, s1 = *(char **)sp1;

	/* walk */
	for (fld = 0, t0 = s0, t1 = s1; ; fld++) {
		while (*s0 != '\0') {	/* next field in first */
			if (*s0 == sep)
				break;
			s0++;
		}
		while (*s1 != '\0') {	/* next field in second */
			if (*s1 == sep)
				break;
			s1++;
		}
		if ((unsigned)(*s0) ^ (unsigned)(*s1))	{ /* not equal --
							   some missymetry */
			fprintf(stderr, "err, not symmetrical fields\n");
			exit(1);
		}

		if (*s0 == '\0')	/* both finished */
			return fldcmp(fld, t0, t1);	/* CMP HERE */
				/* not finished, cut the next token */
		*s0 = *s1 = '\0';
		cmp = fldcmp(fld, t0, t1);		/* CMP HERE */
		*s0 = *s1 = sep;	/* restore separator */
		if (cmp)	/* already know what line is "bigger" */
			return cmp;
		while (*++s0 == sep)
			;
		while (*++s1 == sep)
			;
		t0 = s0, t1 = s1;
	}
}

int fldcmp(int fld, const char *s0, const char *s1)
{
	int i0, i1;
	char c0, c1;

	if (flds[fld] & OPT_N) {	/* numerical comparison */
		i0 = atoi(s0), i1 = atoi(s1);	/* return i0 - i1 will fail on big */
		if (flds[fld] & OPT_R)
			return i1 < i0 ? -1 : i1 > i0 ? 1 : 0;
		return i0 < i1 ? -1 : i0 > i1 ? 1 : 0;
	}
	if (!(flds[fld] & OPT_F))
		return (flds[fld] & OPT_R) ? strcmp(s1, s0) : strcmp(s0, s1);
	return (flds[fld] & OPT_R) ? strcasecmp(s1, s0) : strcasecmp(s0, s1);
}

/* free lines */
void frlines(char *lines[], int linecnt)
{
	while (linecnt--)
		free(*lines++);
}

/* some error checks are skipped */
int main(int argc, char **argv)
{
	int opt;	/* global CLI */
	char *lines[MAXLINES];
	int linecnt;
	int fld;

	/* process CLI */
	for (opt = 0, sep = ' '; --argc && **++argv == '-'; ) {
		while (*++*argv)
			switch (**argv) {
				case 't':	/* field separator */
					sep = *++*argv;
					break;
				case 'k':
					fld = *++*argv - '0';
					while (*++*argv)
						switch (**argv) {
						case 'n':
							flds[fld] |= OPT_N;
							break;
						case 'f':
							flds[fld] |= OPT_F;
							break;
						case 'r':
							flds[fld] |= OPT_R;
							break;
						default:
							printf("unknown option %c\n",
									**argv);
							return 1;
						}
					goto next;

					/* global params 
					 * (substituted by -k for each field) */
				case 'n': opt |= OPT_N; break;	/* numerical */
				case 'f': opt |= OPT_F; break;	/* ignoring case */
				case 'r': opt |= OPT_R; break;	/* reverse */
				default:
					  printf("unknown option %c\n", **argv);
					  return 2;
			}
next:		;
	}

	/* store all options */
	for (fld = 0; fld < MAXFIELDS; fld++)
		flds[fld] |= opt;

	/* read lines */
	if ((linecnt = readlines(lines, MAXLINES)) == -1) {
		fprintf(stderr, "input err\n");
		return 1;
	}
	/* sort */
	qsort(lines, linecnt, sizeof(char *), linecmp);
	/* show */
	prlines(lines, linecnt);
	/* free (not needed, but just as demonstration */
	frlines(lines, linecnt);

	return 0;
}
