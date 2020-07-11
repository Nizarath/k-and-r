/*
 * strend
 */
#include <stdio.h>

int strlen(const char *s)
{
	register int l;

	for (l = 0; *s; s++, l++)
		;
	return l;
}

int strend(const char *s, const char *t)
{
	register const char *rs, *rt;

	/* move backward */
	rs = s, rt = t;
	rs += strlen(rs), rt += strlen(rt);

	/* loop until either start or first mismatch */
	while (rs != s && rt != t && *--rs == *--rt)
		;

	return rt == t ? 1 : 0;
}

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

#define MAX_LINE	100


/* test */

int main(void)
{
	char s[MAX_LINE], t[MAX_LINE];

	while (1) {	/* stop with ^C */
		printf("s: ");
		getln(s, MAX_LINE);
		printf("t: ");
		getln(t, MAX_LINE);
		printf("> %d\n", strend(s, t));
	}

	return 0;
}


