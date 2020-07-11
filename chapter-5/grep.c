/*
 * grep, supports:
 * -v -- invert match
 * -n -- number lines
 * -c -- count instead of lines
 * -i -- ignore case
 */

#include <stdio.h>
#include <string.h>	/* str*str */

/* getline part */

#define MAXLINE		160

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

/* grep */
int main(int argc, char *argv[])
{
	int v, c, n, i;		/* options */
	char ch;
	char *(*parser)(const char *, const char *), *pattern;
	extern char *strstr(const char *, const char *);
	extern char *strcasestr(const char *, const char *);
	char s[MAXLINE];
	int cnt, lineno;

	/* parse set of args */
	for (v = i = c = n = 0; --argc > 0 && **++argv == '-'; )
		while (ch = *++*argv)		/* parse current arg */
			switch (ch) {
				case 'v':	/* invert */
					v = 1;
					break;
				case 'i':	/* ignore case */			
					i = 1;
					break;
				case 'c':	/* count */
					c = 1;
					break;
				case 'n':	/* lineno */
					n = 1;
					break;
				default:	/* unknown */
					printf("grep: unknown option %c\n", ch);
					return 2;
			}

	/* pattern missed */
	if (argc < 1) {
		printf("usage: grep [-cinv] <pattern>\n");
		return 2;
	}

	/* determine parser and pattern */
	pattern = *argv;
	parser = !i ? strstr : strcasestr;

	/* process */
	for (cnt = 0, lineno = 1; getln(s, MAXLINE) > 0; lineno++)
		if ((parser(s, pattern) != NULL) != v) {	/* match */
			cnt++;		/* match count */
			if (c)		/* need only it (-c) */
				continue;
			if (n)
				printf("%d:", lineno);
			printf("%s", s);
		}

	if (c)	/* match count */
		printf("%d\n", cnt);

	return !cnt;
}
