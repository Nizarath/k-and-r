/*
 * entab;
 * replace ' '-only lines with '\t'
 * where possible
 */
#include <stdio.h>

#define MAXLINE	80
#define TAB	8

/* get line */
int getln(char *s, int max)
{
	register int c;
	register char *b;

	for (b = s; (s - b) < (max - 1) &&
		((c = getchar()) != EOF); *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';
	return s - b;
}

/* check spaces */
int lnemp(const char *s)
{
	while (*s) {
		if (*s != ' ' && *s != '\n')
			return 0;
		s++;
	}
	return 1;
}

/* replace empty strings,
 * uses given line length
 */
int entab(char *s, int l)
{
	int tab, sp;
	char end;

	/* not count '\n' */
	end = s[l - 1];	/* preserve end */
	if (end == '\n') 
		l--;
	/* calc */
	tab = l / TAB;
	sp = l % TAB;
	/* replace with tabs */
	while (tab--)
		*s++ = '\t';
	/* replace spaces by separating them
	 * (they are already here)
	 */
	s[sp] = end;
	/* preserved end */
	if (end == '\n')
		s[sp + 1] = '\0';
}	

int main(void)
{
	register int c, l;
	char s[MAXLINE];

	while ((l = getln(s, MAXLINE)) > 0) {
		if (lnemp(s))
			entab(s, l);
		printf("%s", s);
	}
	return 0;
}
