/*
 * fahrenheit program
 */

#include <stdio.h>

#define LOW	0
#define HIGH	300
#define INC	10

int main(void)
{
	int fahr;

	/* header */
	printf("  F | C\n-----------\n");
	/* main work */
	for (fahr = HIGH; fahr >= LOW; fahr -= INC)
		printf("%3d | %5.1f\n", fahr, (5.0 / 9) * (fahr - 32));
	return 0;
}
