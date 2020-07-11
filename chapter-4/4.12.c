/*
 * recursive itoa
 */

#include <stdio.h>

/*
 * f is hack for caller to pass always 1,
 * than it's the highest recursion level (others will have
 * 0), so i will be resetted
 */
void itoa(char *s, int n, int f)
{
	static int i = 0;

	if (n < 0) {	/* called only once */
		s[i++] = '-';
		n = -n;
	}
	if (n / 10)
		itoa(s, n / 10, 0);
	s[i++] = n % 10 + '0';
	s[i] = '\0';
	/* reset i if it was the first time */
	if (f == 1)
		i = 0;
}

/* test */
int main(void)
{
	int n;
	char s[100];

	/* terminate with ^C */
	while (1) {
		printf("n: ");
		scanf("%d", &n);
		itoa(s, n, 1);
		printf("s: %s\n", s);
	}
	
	return 0;
}

