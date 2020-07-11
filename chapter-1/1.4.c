/*
 * celcium program
 */

#include <stdio.h>

#define LOW	-30
#define HIGH	70
#define INC	10

int main(void)
{
	int c;

	/* header */
	printf("  C | F\n-----------\n");
	/* main work */
	for (c = LOW; c <= HIGH; c += INC)
		printf("%3d | %5.1f\n", c, 1.8 * c + 32);
	return 0;
}
