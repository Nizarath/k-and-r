/* simple version of diff */

#include <stdio.h>
#include <stdlib.h>

int fgetln(char *s, int max, FILE *fp)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (max - 1) &&
			(c = getc(fp)) != EOF; *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';

	return s - b;
}

int fputln(char *s, FILE *fp)
{
	for (; *s; s++)
		putc(*s, fp);
	return ferror(fp);
}

#define MAXLINE		1000

int main(int argc, char *argv[])
{
	FILE *fp1, *fp2;
	char s1[MAXLINE], s2[MAXLINE];
	register int l1, l2, ln1, ln2;

	/* check CLI */
	if (argc != 3) {
		fprintf(stderr, "usage: diff <f1> <f2>\n");
		exit(1);
	}

	/* open files */
	fp1 = fopen(argv[1], "r");
	if (fp1 == NULL) {
		fprintf(stderr, "error opening %s\n", argv[1]);
		exit(1);
	}
	fp2 = fopen(argv[2], "r");
	if (fp2 == NULL) {
		fprintf(stderr, "error opening %s\n", argv[2]);
		exit(1);
	}

	/* read in loop */
	for (ln1 = ln2 = 1; ; ln1++, ln2++) {
		l1 = fgetln(s1, MAXLINE, fp1);
		l2 = fgetln(s2, MAXLINE, fp2);

		/* some of files are longer;
		 * may be avoided by always 
		 * if (strcmp(s1, s2)) ...,
		 * but may show empty line for some file
		 */
		if (l1 && !l2)
			printf("> %s %d: %s", argv[1], ln1, s1);
		else if (!l1 && l2)
			printf("> %s %d: %s", argv[2], ln2, s2);
		else if (strcmp(s1, s2))
			printf("> %s %d: %s\n> %s %d: %s\n", 
					argv[1], ln1, s1, argv[2], ln2, s2);
		else if (!l1 && !l2)	/* end of stream */
			break;
		else
			continue;
	}

	exit(0);
}

