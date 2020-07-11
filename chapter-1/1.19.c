/*
 * reverse input lines 
 */

#include <stdio.h>

#define MAXLINE 	80

/*
 * get line,
 * returns length up to '\n'
 */
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

/*
 * reverse string with given
 * length (avoid recalc);
 * the end is '\0' or '\n'
 */
void srev(char *s, int l)
{
	register int i;
	register char c;

	/* check end to avoid reverse '\n' */
	if (s[l - 1] == '\n')
		l--;
	/* reverse */
	for (i = 0; i < (l >> 1); i++) {
		c = s[i];
		s[i] = s[l - i - 1];
		s[l - i - 1] = c;
	}
}

/* main work */
int main(void)
{
	char s[MAXLINE];
	register int l;

	while ((l = getln(s, MAXLINE)) > 0) {
		srev(s, l);
		printf("%s", s);
	}
	return 0;
}
