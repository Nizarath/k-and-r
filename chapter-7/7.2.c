/*
 * prints each symbol as it is for printable ones,
 * or as hex code in other case;
 * it's actually strange behaviour for '\n' symbols, although it's demonstrative
 */
#include <stdio.h>

#define DEF_MAX_POS	80

int main(int argc, char *argv[])
{
	register int c, pos, maxpos;

	maxpos = DEF_MAX_POS;	/* default */

	if (argc > 1)		/* error check is skipped */
		maxpos = atoi(*(argv + 1));

	for (pos = 0; (c = getchar()) != EOF; pos++) {
		printf(!isprint(c) ? "0x%x" : "%c", c);
		if (pos == maxpos - 1) {
			printf("0x%x", '\n');
			pos = -1;
		}
	}
	return 0;
}

