/*
 * squeeze
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

/*
 * remove all symbols of s2
 * from s1;
 * O(N^2)
 */
void squeeze(char *s1, const char *s2)
{
	register int i, j;

	for (i = j = 0; s1[i]; i++)
		if (!(bel(s2, s1[i])))
			s1[j++] = s1[i];
	s1[j] = '\0';
}

/* getline */
int getln(char *s, int max)
{
	register int c;
	register char *b;

	for (b = s; (s - b) < (max - 1) &&
		(c = getchar()) != EOF; *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';
	return s - b;
}

int main(void)
{
	char s1[MAXLINE], s2[MAXLINE];

	/* terminate the program with ^C or such */
	while (1) {
		getln(s1, MAXLINE);
		getln(s2, MAXLINE);
		squeeze(s1, s2);
		printf("> %s\n", s1);
	}
	return 0;
}
