#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int check_name(char *argv)
{
	int big;

	while (!isalpha(*argv))
		argv++;
	for (big = isupper(*argv++); *argv; argv++)
		if (isalpha(*argv) && (isupper(*argv) != big))
			return -1;
	return !!big;
}
	

int main(int argc, char *argv[])
{
	int big, c, (*fp[])(int) = { tolower, toupper };

	if ((big = check_name(*argv)) == -1) {
		fprintf(stderr, "name must be written in one case\n");
		exit(1);
	}
	while ((c = getchar()) != EOF)
		putchar((*fp[big])(c));
	return 0;
}
