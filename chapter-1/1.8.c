/*
 * count special symbols
 */

#include <stdio.h>

int main(void)
{
	int s, t, n;	/* interested */
	int c;		/* getchar */

	for (s = t = n = 0; (c = getchar()) != EOF; )
		switch (c) {
			case ' ': s++; break;
			case '\t': t++; break;
			case '\n': n++;
		}
	printf("' ': %d, '\\t': %d, '\\n': %d\n",
			s, t, n);
	return 0;
}
