/*
 * parse (), {}, []
 * in C-programs
 */
#include <stdio.h>

#define CHCKEOF(c)	{if((c) == EOF)return 0;}

/******************************* 
 * bracket stack,
 * is used for all types:
 * - parentheses; ()
 * - brackets; []
 * - figure br.; {}
 */

/* match state of stack */
#define OK	0	/* brackets match */
#define BAD	1	/* brackets don't match */

/* stack representation */
struct bst {
	int lev;	/* current stack depth */
	int state;	/* match state */
};
/*******************************/

/* main work */
int main(void)
{
	register int c;
	int br_big, br_small;
	int p_st, b_st, f_st;

	/*
	 * init: not in brackets,
	 * 3 stacks zeroed
	 */
	br_big = br_small = 0;
	p_st = b_st = f_st = 0;

	/* read and process */
	while ((c = getchar()) != EOF) {
		
		/* 
		 * (/ -> *) + outside == commentary:
		 * ignore all
		 */
		if (c == '/' && !(br_big + br_small)) {
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
			} else
				CHCKEOF(c);

		/* check '\' (may break brackets' checks
		 * in cases like '\'' and '\"' (odd count))
		 */
		} else if (c == '\\') {
			if (br_small) {
				c = getchar();	/* skip without switching */
				CHCKEOF(c);
			}

		/* brackets check */
		} else if (c == '\'') {	/* '' */
			br_small ^= 1;		/* invert state */

		} else if (c == '\"') {	/* "" */
			br_big ^= 1;		/* invert state */

		/* finally, the main parser of brackets --
		 * outside of comments and string literals */
		} else if (br_big + br_small == 0) { 
			switch (c) {
				/* left */
				case '(': p_st++; break;
				case '[': b_st++; break;
				case '{': f_st++; break;
				/* right */
				case ')': p_st--; break;
				case ']': b_st--; break;
				case '}': f_st--;
			}
		}
	}

	/* check results */
	if (p_st | b_st | f_st)
		printf("mistakes detected\n");

	return 0;
}
