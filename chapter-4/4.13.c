/*
 * recursive reverse
 */

#include <stdio.h>

int strlen(const char *s)
{
	register int l;

	for (l = 0; *s; s++, l++)
		;
	return l;
}

void reverse(char *s)
{
	char c;
	int pos;

	/* check if > 1 symbol */
	pos = strlen(s) - 1;
	if (pos > 0) {	/* need recursion */
		c = s[pos];
		s[pos] = '\0';
		reverse(s + 1);
		/* swap finally */
		s[pos] = s[0];
		s[0] = c;
	}
}

#define MAXLINE 	100

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

/* test */
int main(void)
{
	char s[MAXLINE];

	while (getln(s, MAXLINE) > 0) {
		reverse(s);
		printf("> %s\n", s);
	}

	return 0;
}
