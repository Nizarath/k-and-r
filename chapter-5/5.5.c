/*
 * strn* functions
 */

#include <stdio.h>

size_t strnlen(const char *s, size_t n)
{
	register size_t l;

	for (l = 0; l < n && *s; s++, l++)
		;
	return l;
}

void strncpy(char *d, const char *s, size_t n)
{
	while (n-- > 0 && (*d++ = *s++))
		;
}

void strncat(char *d, const char *s, size_t n)
{
	while (*d)
		d++;
	while (n-- > 0 && (*d++ = *s++))
		;
}

int strncmp(const char *s0, const char *s1, size_t n)
{
	for (; n > 0 && *s0 == *s1; s0++, s1++, n--)
		if (*s0 == '\0' || n <= 0)
			return 0;
	return *s0 - *s1;
}

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

#define MAX_LINE	100


/* test */
int main(void)
{
	char s0[MAX_LINE], s1[MAX_LINE], tmp[MAX_LINE];
	int n;

	while (1) {	/* stop with ^C */
		printf("s0: ");
		getln(s0, MAX_LINE);
		printf("s1: ");
		getln(s1, MAX_LINE);
		printf("n: ");
		scanf("%u", &n);
		getchar();
		strcpy(tmp, s0);	/* backup: s0 will be modified */
		/* perform all with originals */
		printf("strnlen: %u %u\n", strnlen(s0, n), strnlen(s1, n));
		printf("strncmp: %d\n", strncmp(s0, s1, n));
		strncpy(s0, s1, n);
		printf("strncpy: %s\n", s0);
		strncat(tmp, s1, n);
		printf("strncat: %s\n\n", tmp);
	}

	return 0;
}


