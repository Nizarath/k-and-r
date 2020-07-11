/*
 * any
 */

#include <stdio.h>

#define MAXLINE		80

/* check belonging, if s is empty --
 * no one belongs, even '\0' */
int bel(const char *s, char c)
{
	while (*s)
		if (*s++ == c)
			return 1;
	return 0;
}

/* getline,
 * but this time without '\n'
 * (to avoid clashing)
 */
int getln(char *s, int max)
{
	register int c;
	register char *b;

	for (b = s; (s - b) < (max - 1) &&
		(c = getchar()) != EOF && c != '\n'; *s++ = c)
		;
	*s = '\0';
	return s - b;
}

/* any */
int any(const char *s1, const char *s2)
{
	const char *t;

	for (t = s1; *s1; s1++)
		if (bel(s2, *s1))
			return s1 - t;
	return -1;
}

int main(void)
{
	char s1[MAXLINE], s2[MAXLINE];
	int pos;

	/* terminate the program with ^C or such */
	while (1) {
		/* read strings */
		getln(s1, MAXLINE);
		getln(s2, MAXLINE);
		/* show pos */
		pos = any(s1, s2);
		if (pos == -1)
			printf("-\n");
		else
			printf("> %d\n", pos);
	}
	return 0;
}
