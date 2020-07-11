/*
 * cat -- libc version with reading for bytes
 */
#include <stdio.h>
#include <stdlib.h>

void cat(FILE *fp)
{
	int c;

	while ((c = getc(fp)) != EOF)
		putc(c, stdout);
}

int main(int argc, char *argv[])
{
	FILE *fp;

	if (argc == 1)
		cat(stdin);
	else
		while (--argc && *++argv) {
			fp = fopen(*argv, "r");
			if (fp == NULL) {
				fprintf(stderr, "error open %s\n", *argv);
				exit(1);
			}
			cat(fp);
			fclose(fp);
		}
	exit(0);
}
