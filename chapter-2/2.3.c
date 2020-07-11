/*
 * hex string -> number
 */

#include <stdio.h>

/* line params */
#define MAXLINE		80

/* macros for hex */
#define isdigit(x)	((x) >= '0' && (x) <= '9')
#define islalpha(x)	((x) >= 'a' && (x) <= 'f')
#define isualpha(x)	((x) >= 'A' && (x) <= 'F')

/* 16-digit -> 10-digit */
int x2d(int x)
{
	/* 0..9 */
	if (isdigit(x))
		return x - '0';
	/* a-f */
	if (islalpha(x))
		return x + 10 - 'a';
	/* A-F */
	if (isualpha(x))
		return x + 10 - 'A';
	/* some crap */
	return -1;
}

/* parse string and give num */
int htoi(const char *s)
{
	register int n, x;

	/* check prefix 0x or 0X */
	if (*s == '0' && (s[1] == 'x' || s[1] == 'X'))
		s += 2;
	/* parse */
	for (n = 0; *s; s++) {
		x = x2d(*s);	/* parse */
		if (x == -1)	/* incorrect */
			break;
		/* calc */
		n = (n << 4) + x;
	}
	return n;
}
		
/* get line */
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
 * main work:
 * 1. get line;
 * 2. transform it into hex;
 * 3. print it;
 */
int main(void)
{
	char s[MAXLINE];

	while (getln(s, MAXLINE) > 0)
		printf("= %d\n", htoi(s));
	return 0;
}
