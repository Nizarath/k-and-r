/*
 * cutting blank lines
 */
#include <stdio.h>

int main(void)
{
	int c, nl, ns; /* new line marker, count of spaces */

	for (nl = 1; (c = getchar()) != EOF; nl = (c == '\n')) {
		if (nl) {
			/* we need count ' ', since may be "    s", need
			 * output "    s" (not to lose 's')
			 */
			for (ns = 0; c == ' '; c = getchar())
				ns++;
			/* full empty string -- the only case need to
			 * place 1 space
			 */
			if (c == '\n' && ns)
				ns = 1;
			while (ns--)
				putchar(' ');
			/* immediatly exit if EOF to avoid it's showing
			 * with outside putchar() */
			if (c == EOF)
				return 0;
		}
		putchar(c);
	}
	return 0;
}
