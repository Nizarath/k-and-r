#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE		100	/* one line length */

int getln(char *s, int max)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (max - 1) &&
			(c = getchar()) != EOF && c != '\n'; *s++ = c)
		;
	*s = '\0';

	return s - b;
}

int readlines(char *lines[], int maxlines)
{
	char buf[MAXLINE], *alloc;
	int pos, top;

	for (top = 0; getln(buf, MAXLINE) > 0; ) {
		/* prepare dynamic buf */
		alloc = malloc(strlen(buf) + 1);
		if (alloc == NULL)
			return -1;
		strcpy(alloc, buf);

		/* get pos */
		pos = top++ % maxlines;
		if (top > maxlines)
			free(lines[pos]);

		/* place */
		lines[pos] = alloc;
	}
	return top;
}

void freelines(char *lines[], int maxlines, int top)
{
	int pos;

	/* free lines themselves */
	if (top <= maxlines)
		for (pos = 0; pos < top; pos++)
			free(lines[pos]);
	else {
		for (pos = top % maxlines; pos < maxlines; pos++)
			free(lines[pos]);
		for (pos = 0; pos < top % maxlines; pos++)
			free(lines[pos]);
	}

	/* free array */
	free(lines);
}

int main(int argc, char *argv[])
{
	int maxlines, top, pos;
	char **lines;

	maxlines = 10;		/* default */
	while (--argc && *++argv)	/* parse CLI */
		while (*++*argv)
			switch (**argv) {
			case 'n':
				if (*++argv == NULL) {
					printf("-n argument isn't specified\n");
					return 1;
				}
				maxlines = atoi(*argv);
				if (maxlines <= 0) {
					printf("n must be >0\n");
					return 1;
				}
				break;
			case 'h':
				printf("usage: tail [-n <maxlines>]\n");
				return 0;
			default:
				printf("unknown option: %c\n", **argv);
				return 1;
			}

	/* alloc buf for lines */
	lines = malloc(sizeof(char *) * maxlines);
	if (lines == NULL) {
		printf("fail alloc lines\n");
		return -1;
	}

	/* process input and fill lines */
	if ((top = readlines(lines, maxlines)) != -1) {
		/* show the lines */
		if (top <= maxlines)
			for (pos = 0; pos < top; pos++)
				printf("%s\n", lines[pos]);
		else {
			for (pos = top % maxlines; pos < maxlines; pos++)
				printf("%s\n", lines[pos]);
			for (pos = 0; pos < top % maxlines; pos++)
				printf("%s\n", lines[pos]);
		}
		return 0;
	} else {
		printf("input fail\n");
		return -1;
	}

	/* memory will be free'd anyway, but
	 * here is for demonstration */
	freelines(lines, maxlines, top);

	return 0;
}
