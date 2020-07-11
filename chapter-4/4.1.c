/*
 * strrindex(s,t);
 */

#include <stdio.h>

/* str length */
int strlen(const char *s)
{
	register int l;

	for (l = 0; *s; s++, l++)
		;
	return l;
}

/* main core */
int strrindex(const char *s, const char *t)
{
	register int i, j, k, lt;

	/* check emptiness here to avoid loop checks */
	if (*t == '\0' || *s == '\0')
		return -1;
	
	/* main loop */
	lt = strlen(t) - 1;	/* remember for inner loop */
	for (i = strlen(s) - 1; i >= 0; i--) {
		/* start from the end of pattern, remember current */
		for (j = i, k = lt; k >= 0 && s[j] == t[k]; j--, k--)
			;
		if (k == -1)	/* return START of pattern in main str */
			return i - lt;
	}
	return -1;
}

/* getline */

#define MAXLINE 	80

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
	char s[MAXLINE], t[MAXLINE];

	/* stop with ^C */
	while (1) {
		printf("s: ");
		getln(s, MAXLINE);
		printf("t: ");
		getln(t, MAXLINE);
		printf("> %d\n\n", strrindex(s, t));
	}

	return 0;
}
