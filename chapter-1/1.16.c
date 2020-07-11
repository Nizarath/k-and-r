/*
 * finding the longest line
 */
#include <stdio.h>

#define MAXLINE		10

/*
 * read line from stdin to 
 * buffer of lim size,
 * return length up to '\n'
 */
int getln(char *s, int lim)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (lim - 1) &&
			(c = getchar()) != EOF; *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';
	return s - b;
}

/* main work */
int main(void)
{
	char s[MAXLINE];
	int len, full_len, maxlen;

	maxlen = 0;
	while ((len = getln(s, MAXLINE)) > 0) {
		/* check for BIG string */
		full_len = 0;
		while (len == MAXLINE - 1 && s[len - 1] != '\n') {
			full_len += len;
			len = getln(s, MAXLINE);
		}
		full_len += len;
		/* choose max */
		if (full_len > maxlen)
			maxlen = full_len;
	}
	printf("maxlen = %d\n", maxlen);
	return 0;
}
