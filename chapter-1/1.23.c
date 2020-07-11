/*
 * remove comments from the
 * C program
 */

#include <stdio.h>

#define MAXLINE		80

#define CHCKEOF(c)	{if((c) == EOF)return 0;}

/* main work */
int main(void)
{
	int br_small, br_big;	/* inside brackets */
	register int c;	/* read char */

	/* 
	 * init:
	 * - outside of "", '';
	 */
	br_small = br_big = 0;
	while ((c = getchar()) != EOF) {

		/* check '\' (may break brackets' checks
		 * in cases like '\'' and '\"' (odd count))
		 */
		if (c == '\\') {
			putchar(c);
			if (br_small) {
				c = getchar();	/* show without switching */
				CHCKEOF(c);
				putchar(c);
			}

		/* brackets check */
		} else if (c == '\'') {	/* '' */
			br_small ^= 1;		/* invert state */
			putchar(c);

		} else if (c == '\"') {	/* "" */
			br_big ^= 1;		/* invert state */
			putchar(c);

		/* (/ -> *) + outside == commentary */
		} else if (c == '/' && !(br_big + br_small)) {
			/* real commentary start */
			if ((c = getchar()) == '*') {
				do {	
					/* parse until end */
					while ((c = getchar()) != '*')
						CHCKEOF(c);
					/* skip last *s */
					while ((c = getchar()) == '*')
						CHCKEOF(c);
				} while (c != '/');
			/* not commentary */
			} else {
				CHCKEOF(c);
				putchar(c);
			}

		/* other symbol */
		} else
			putchar(c);
	}
	printf("%d %d\n", br_small, br_big);
	return 0;
}
