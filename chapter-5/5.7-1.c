/*
 * sort program
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>	/* malloc */

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

int readlines(char *lineptr[], int maxlines)
{
	int lcnt;
	char s[MAXLINE], *sp;

	/* read new line to buffer */
	lcnt = 0;
	while (getln(s, MAXLINE) > 0) {
		/* store it into array */
		if (lcnt >= maxlines)		/* too many */
			return -1;
		sp = malloc(strlen(s) + 1);
		if (sp == NULL)			/* alloc fail */
			return -1;
		strcpy(sp, s);
		lineptr[lcnt++] = sp;
	}

	return lcnt;
}

/****** compare strings ********/

int scmp(const void *s0, const void *s1)
{
	return strcmp(*(char **)s0, *(char **)s1);
}

/****** show them ********/
void prlines(char *lineptr[], int maxlines)
{
	while (maxlines--)
		printf("%s", *lineptr++);
}

/******* free them *******/
void freelines(char *lineptr[], int maxlines)
{
	while (maxlines--)
		free(*lineptr++);
}

/* main test */
int main(void)
{
	int lcnt;
	char *lineptr[MAXLINES];

	if ((lcnt = readlines(lineptr, MAXLINES)) > 0) { /* input ok, may sort */
		qsort(lineptr, lcnt, sizeof(char *), scmp);
		printf("> qsort:\n");
		prlines(lineptr, lcnt);
		/* free mem */
		freelines(lineptr, lcnt);	/* redundant, not needed */

		return 0;
	} else {
		printf("input fail\n");
		
		return -1;
	}
}

