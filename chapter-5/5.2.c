/*
 * getint/getfloat
 */

#include <stdio.h>

/* pow */

int pow(int x, int y)
{
	register int p;

	for (p = 1; y--; p *= x)
		;
	return p;
}

/* getch/ungetch */

#define MAX_BUF		200

int buf[MAX_BUF];
int bp = 0;		/* next FREE pos */

int getch(void)
{
	return (bp <= 0) ? getchar() : buf[--bp];
}

void ungetch(int c)
{
	if (bp < MAX_BUF)
		buf[bp++] = c;
	else
		printf("ungetch: buffer fill\n");
}

/* main work */
int getfloat(float *pn)
{
	int c, sign, t, p, exp_sign, exp_pow;

	while (isspace(c = getch()))	/* skip spaces */
		;

	/* non digit */
	if (!isdigit(c) && c != '+' && c != '-' && c != EOF && c != '.') {
		ungetch(c);
		return 0;
	}

	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-') {	/* potential num's sign */
		t = c;
		c = getch();
		if (!isdigit(c) && c != '.') {	/* not [+-][.0-9] */
			ungetch(c);
			ungetch(t);
			return 0;
		}
	}

	/* integer part */
	for (*pn = 0; isdigit(c); c = getch())
		*pn = *pn * 10 + c - '0';
	/* check fraction */
	if (c == '.')
		c = getch();
	/* fraction part */
	for (p = 1; isdigit(c); c = getch()) {
		p *= 10;
		*pn = *pn * 10 + c - '0';
	}

	*pn = (sign * *pn) / p;

	/* exponent */
	if (c == 'e' || c == 'E') {
		t = c;
		c = getch();
		/* [eE][^0-9+-] -- incorrect */
		if (c != '+' && c != '-' && !isdigit(c)) {
			ungetch(c);		/* [^0-9+-] -> stream */
			ungetch(t);		/* [eE] -> stream */

			return 1;
		}
		/* check '+'/'-' */
		exp_sign = (c == '-') ? -1 : 1;
		if (c == '+' || c == '-') {
			p = c;  	/* temp */
			c = getch();
			if (!isdigit(c)) {	/* [eE][+-][^0-9] -- incorrect */
				ungetch(c);	/* [^0-9] -> stream */
				ungetch(p);	/* [+-] -> stream */
				ungetch(t);	/* [eE] -> stream */

				return 1;
			}
		}
		/* calc pow */
		for (exp_pow = 0; isdigit(c); c = getch())
			exp_pow = 10 * exp_pow + c - '0';
		/* finish */
		if (exp_sign > 0)
			*pn = *pn * pow(10, exp_pow);
		else
			*pn = *pn / pow(10, exp_pow);
	}
	ungetch(c);

	return c;
}

/* test */
int main(void)
{
	float f;

	while (getfloat(&f) > 0)
		printf("> %f\n", f);
	return 0;
}
