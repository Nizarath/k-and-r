/*
 * detab:
 * \t -> spaces
 */

#include <stdio.h>

#define TAB	8

int main(void)
{
	register int c, pos;

	for (pos = 0; (c = getchar()) != EOF; pos++)
		if (c == '\t') {
			/* reuse c */
			c = TAB - pos % TAB;	/* how many spaces need */
			pos += c - 1;		/* update to avoid i */
			while (c--)
				putchar(' ');
		} else {
			if (c == '\n')		/* update pos */
				pos = -1;
			putchar(c);
		}
	return 0;
}
