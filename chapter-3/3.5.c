/*
 * itob
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

/* supports up to 26th numerical system (sic!) */
void itob(int n, char *s, int b)
{
	/* bases for digits, store them in data section instead of stack */
	static char bases[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y', 'z'
	};
	int i, sign, t;

	if ((sign = n) < 0)	/* process sign */
		n = -n;
	i = 0;
	do {			/* choose right digit -- will be reversed */
		t = n % b;
		s[i++] = bases[t > 0 ? t : -t];
	} while (n /= b);
	if (sign < 0)		/* if negative, append - */
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

/* main work */
int main(void)
{
	int n, b;
	char s[100];

	/* stop with ^C */
	while (1) {
		printf("b: ");
		scanf("%d", &b);
		printf("n: ");
		scanf("%d", &n);
		itob(n, s, b);
		printf("> %s\n\n", s);
	}

	return 0;
}
