/*
 * setbits
 */

#include <stdio.h>

/**************** INVERT BITS PART ****************/
/*
 * invert x[p; p-n+1]
 */
int invert(unsigned x, int p, int n)
{
	return x ^ (((1 << n) - 1) << (p - n + 1));
}

/****************** STR -> NUM PART *************/

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

/* simple decimal fetch */
int atoi(const char *s)
{
	register int n;

	for (n = 0; *s && isdigit(*s); s++)
		n = 10 * n + *s - '0';
	return n;
}

/* hexademical fetch */
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

/***************** GETLINE PART ***************/

#define MAXLINE		80
		
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

/******************************/
/*
 * main work:
 * 1. read string -> transform to x (hex);
 * 2. read 2 strings -> transform to p and n (decimal);
 * 3. calc X;
 * 4. print it;
 */
int main(void)
{
	char s[MAXLINE];
	unsigned x;
	int p, n;

	/* stop the program by ^C */
	while (1) {
		printf("x: ");		/* read x */
		getln(s, MAXLINE);
		x = htoi(s);
		printf("p: ");		/* read p and n */
		getln(s, MAXLINE);
		p = atoi(s);
		printf("n: ");
		getln(s, MAXLINE);
		n = atoi(s);
		/* calc */
		x = invert(x, p, n);
		printf("> %x\n", x);
	}
	return 0;
}
