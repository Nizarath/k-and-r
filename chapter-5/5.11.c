/*
 * detab, place tabulations until tab borders
 * in the positions given as list of CLI parameters
 */

#include <stdio.h>

#define TAB		8
#define MAXPOS		100

int tab_default[] = {
	0, 5, 10
};

#define NARR(arr)	(sizeof(arr) / sizeof((arr)[0]))

int main(int argc, char *argv[])
{
	register int c, pos;
	int tabs[MAXPOS], top, i;

	if (argc == 1)
		for (top = 0; top < NARR(tab_default); top++)
			tabs[top] = tab_default[top];
	/* read CLI:
	 * $ detab <pos1> [pos2] [pos3] ... [posN]
	 */
	else
		for (top = 0; --argc && *++argv; )
			tabs[top++] = atoi(*argv);
		
	/* read input */
	for (pos = i = 0; (c = getchar()) != EOF; pos++)
		if (pos == tabs[i] && i < top) {
			putchar(c);
			/* reuse c */
			c = TAB - pos % TAB;	/* how many spaces need */
			pos += c - 1;		/* update to avoid i */
			while (c--)
				putchar(' ');
			i++;
		} else {
			if (c == '\n') {	/* update pos */
				pos = -1;
				i = 0;
			}
			putchar(c);
		}
	return 0;
}
