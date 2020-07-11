/*
 * atof with exp support
 */

#include <stdio.h>

#define isspace(c)	((c) == ' ' || (c) == '\t')
#define isdigit(c)	((c) >= '0' && (c) <= '9')

/* x^y */
int pow(int x, int y)
{
	register int p;

	for (p = 1; y--; p *= x)
		;
	return p;
}

double atof(const char *s)
{
	double d, p;
	int sign, exp, exp_sign;

	/* skip spaces */
	while (isspace(*s))
		s++;

	/* check sign */
	sign = 1;
	if (*s == '-') {
		sign = -1;
		s++;
	} else if (*s == '+')
		s++;

	/* main loop */
	for (d = 0.0; isdigit(*s); s++)
		d = 10 * d + *s - '0';

	/* check . */
	p = 1.0;	/* here for correct end */
	if (*s == '.') {
		for (s++; isdigit(*s); s++) {
			d = 10 * d + *s - '0';
			p *= 10;
		}
	}

	/* check exp, independently of . */
	if (*s == 'e' || *s == 'E') {
		s++;

		/* get exponent sign */
		exp_sign = 1;
		if (*s == '-') {
			exp_sign = -1;
			s++;
		} else if (*s == '+')
			s++;

		/* accum exp (cann't call atoi -- since
		 * cann't skip spaces etc. */
		for (exp = 0; isdigit(*s); s++)
			exp = 10 * exp + *s - '0';

		/* calc 10^exp */
		exp = pow(10, exp);
		d = (exp_sign == 1) ? (d * exp) : (d / exp);
	}

	/* res = f(d, sign, pow); */
	return sign * (double)d / p;
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
	char s[MAXLINE];

	while (getln(s, MAXLINE) > 0)
		printf("> %f\n", atof(s));

	return 0;
}

