/*
 * folding lines
 */

#include <stdio.h>

#define MAXLINE		10

/*
 * get line
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
 * remove blank tail
 */
void trunc(char *s, int l)
{
	register int i;

	for (i = l - 1; i >= 0; i--)
		if (s[i] != ' ' && s[i] != '\t')
			break;
	/* cann't trunc -- fully blank */
	if (!i)
		return;
	/* trunc */
	s[i + 1] = '\n';
	s[i + 2] = '\0';
}

int main(void)
{
	char s[MAXLINE];
	register int l;

	/* read lines */
	while ((l = getln(s, MAXLINE)) > 0) {
		/* check if too big */
		if (l == MAXLINE - 1 && s[l - 1] != '\n')
			trunc(s, l);	/* rm blank tail */
		/* output */
		printf("%s", s);
	}
	return 0;
}
