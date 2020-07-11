/*
 * fahrenheit program
 */

#include <stdio.h>

#define LOW	0
#define HIGH	300
#define INC	10

float f2c(int f)
{
	return (5.0f / 9) * (f - 32);
}

int main(void)
{
	int fahr;

	/* header */
	printf("  F | C\n-----------\n");
	/* main work */
	for (fahr = LOW; fahr <= HIGH; fahr += INC)
		printf("%3d | %5.1f\n", fahr, f2c(fahr));
	return 0;
}
