/*
 * getint/getfloat
 */

#include <stdio.h>

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
int getint(int *pn)
{
	int c, sign, t;

	while (isspace(c = getch()))	/* skip spaces */
		;
	if (!isdigit(c) && c != '+' && c != '-' && c != EOF) {
		ungetch(c);
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-') {
		t = c;
		c = getch();
		if (!isdigit(c)) {	/* not [+-][0-9] */
			ungetch(c);
			ungetch(t);
			return 0;
		}
	}

	for (*pn = 0; isdigit(c); c = getch())
		*pn = *pn * 10 + c - '0';
	*pn *= sign;
	ungetch(c);

	return c;
}

/* test */
int main(void)
{
	int n;

	while (getint(&n) > 0)
		printf("> %d\n", n);
	return 0;
}
