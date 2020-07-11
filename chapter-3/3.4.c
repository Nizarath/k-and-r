/*
 * itoa
 */

#include <stdio.h>

int strlen(const char *s)
{
	register int n;

	for (n = 0; *s; s++)
		n++;
	return n;
}

void reverse(char *s)
{
	register int i;
	register char c;
	int l;

	for (l = strlen(s), i = 0; i < (l >> 1); i++) {
		c = s[i];
		s[i] = s[l-i-1];
		s[l-i-1] = c;
	}
}

void itoa(int n, char *s)
{
	int i, sign, t;

	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		t = n % 10;
		s[i++] = ((t > 0) ? t : -t) + '0';
	} while (n /= 10);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

/* test */
int main(void)
{
	char s[100];
	int n;

	/* stop with ^C */
	while (1) {
		scanf("%d", &n);	/* num in */
		itoa(n, s);		/* calc */
		printf("> %s\n\n", s);	/* show */
	}

	return 0;
}
