/*
 * print words one per line, ignores empty lines
 * (I think it's desirable)
 */

#include <stdio.h>

#define INW	1
#define OUTW	0

int main(void)
{
	int c, state;

	state = OUTW;
	while ((c = getchar()) != EOF) {
		if (c == ' ' || c == '\t' || c == '\n') {
			if (state == INW)
				putchar('\n');
			state = OUTW;
		} else {
			state = INW;
			putchar(c);
		}
	}
	return 0;
}
