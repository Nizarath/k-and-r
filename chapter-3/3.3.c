/*
 * expand
 */

#include <stdio.h>

#define islalpha(c)	((c) >= 'a' && (c) <= 'z')
#define isualpha(c)	((c) >= 'A' && (c) <= 'Z')
#define isdigit(c)	((c) >= '0' && (c) <= '9')

int range_ok(const char *s)
{
	return ((islalpha(*(s - 1)) && islalpha(*(s + 1))) ||
		(isualpha(*(s - 1)) && isualpha(*(s + 1))) ||
		(isdigit(*(s - 1)) && isdigit(*(s + 1)))) &&
			(*(s - 1) < *(s + 1));
}

/*
 * expand(s1, s2)
 */
void expand(const char *s1, char *s2)
{
	register int i, c;

	for (i = 0; s1[i]; i++) {
		/* it's easier to check '-' firstly */
		if (s1[i] == '-' && i && s1[i + 1]) {
			if (range_ok(&s1[i])) {	/* expand */
				for (c = s1[i - 1] + 1; c <= s1[i + 1]; c++)
					*s2++ = c;
				i++;	/* skip '-' */
			} else
				*s2++ = '-';
		} else
			*s2++ = s1[i];
	}
	*s2 = '\0';
}

/* get line */

#define MAXLINE		80

int getln(char *s, int max)
{
	register int c;
	register const char *b;

	for (b = s; (s - b) < (max - 1) &&
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
	char s1[MAXLINE], s2[MAXLINE * 10];

	/* stop the program with ^C */
	while (1) {
		printf("s1: ");
		getln(s1, MAXLINE);
		expand(s1, s2);
		printf("> %s", s2);
	}

	return 0;
}
