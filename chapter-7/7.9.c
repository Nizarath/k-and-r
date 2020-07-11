#include <stdio.h>

int toupper1(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= ('a' - 'A');
	return c;
}

static char arr[256];	/* ascii */

int init_toupper2(void)
{
	register int i;

	for (i = 0; i < 97; i++)
		arr[i] = i;
	for (i = 97; i < 122; i++)
		arr[i] = i - ('a' - 'A');
	for (i = 123; i < 256; i++)
		arr[i] = i;
}

int toupper2(int c)
{
	return arr[c];
}

static int (*fun)(int) = toupper1;

void pr(FILE *fp)
{
	register int c;

	while ((c = getc(fp)) != EOF)
		putc((*fun)(c), stdout);
}

int main(int argc, char *argv[])
{
	FILE *fp;

	if (argc > 1 && **(argv + 1) == '-') {
		argv++;
		fun = toupper2;
		argc--;
		init_toupper2();
	}

	if (argc == 1)
		pr(stdin);
	else {
		while (--argc && *++argv) {
			fp = fopen(*argv, "r");
			if (fp == NULL) {
				fprintf(stderr, "error fopen() %s\n", *argv);
				continue;
			}
			pr(fp);
			fclose(fp);
		}
	}

	return 0;
}
	

