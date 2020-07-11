/*
 * truncate blank tails and empty lines
 */

#include <stdio.h>

#define MAXLINE	80

/* getline */
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
 * trunc line:
 * " ab" -> " ab"
 * "ab " -> "ab"
 * "" -> (removed)
 * "abab  " -> "abab";
 *
 * line must be '\0'-terminated
 */
void trunc(char *s)
{
	register char *t;

	/* empty string */
	if (*s == '\n') {
		*s = '\0';
		return;
	}
	/* seek for tail */
	for (t = s; *t; t++) {
		/* potential blank tail: */
		if (*t == ' ' || *t == '\t') {
			for (s = t; *t != '\n' && *t != '\0'; t++)
				if (*t != ' ' && *t != '\t')
					goto next; /* better than check outside */
			*s = *t;
			s[1] = '\0'; /* for the case *s == '\n' */
			return;
		}
	next:
		;
	}
}

/* main work */
int main(void)
{
	char s[MAXLINE];

	while (getln(s, MAXLINE) > 0) {
		trunc(s);
		printf("%s", s);
	}
	return 0;
}
