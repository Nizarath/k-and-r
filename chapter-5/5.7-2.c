/*
 * sort program
 */

#include <stdio.h>
#include <string.h>

/******** one string input **********/

int getln(char *s, int max)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (max - 1) &&
			(c = getchar()) != EOF; *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';

	return s - b;
}

#define MAXLINE		100

/********** array of strings **********/

#define MAXLINES	1000000

int readlines(char lineptr[][MAXLINE], int maxlines)
{
	int lcnt;

	/* read new line to buffer */
	lcnt = 0;
	for (lcnt = 0; getln(lineptr[lcnt], MAXLINE) > 0; lcnt++)
		if (lcnt >= maxlines)		/* too many */
			return -1;

	return lcnt;
}

/****** show them ********/
void prlines(char lineptr[][MAXLINE], int maxlines)
{
	while (maxlines--)
		printf("%s", *lineptr++);
}

/* placed into data section to avoid stack smashes on big counts,
 * since it's two dimensional array, memory-consumpting */
char lineptr[MAXLINES][MAXLINE];

/* main test */
int main(void)
{
	int lcnt;

	if ((lcnt = readlines(lineptr, MAXLINES)) > 0) { /* input ok, may sort */
		qsort(lineptr, lcnt, MAXLINE, strcmp);
		printf("> qsort:\n");
		prlines(lineptr, lcnt);

		return 0;
	} else {
		printf("input fail\n");
		
		return -1;
	}
}

