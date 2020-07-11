/*
 * strcat
 */
#include <stdio.h>

void strcat(char *s, const char *t)
{
	/* find the end */
	while (*s)
		s++;
	/* t -> s */
	while (*s++ = *t++)
		;
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

	/* stop with ^C */
	while (1) {
		printf("t: ");
		getln(t, MAX_LINE);
		printf("s: ");
		getln(s, MAX_LINE);
		strcat(s, t);
		printf("> %s\n", s);
	}

	return 0;
}
