/*
 * output all lines with the length > 80
 */

#include <stdio.h>

#define MAXLINE 	80

/*
 * get the line in buf[lim],
 * return its file length
 */
int getln(char *s, int lim)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (lim - 1) &&
			(c = getchar()) != EOF; *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';
	return s - b;
}

/* main work */
int main(int argc, char *argv[])
{
	char s[MAXLINE + 1];
	int l;

	/* while lines are present */
	while ((l = getln(s, MAXLINE + 1)) > 0)
		/* this line is too big:
		 * | a | b | c | d | \0 |
		 */
		if (l == MAXLINE && s[l - 1] != '\n') {
			printf("%s", s);
			/* show parts until:
			 * - | a | b | \n | \0 | .. | or
			 *   | a | b |  c | \n | \0 | -- EOL is found, 
			 *   then it's not important what's length
			 * - | a | b | \0 | .. | .. | -- EOF is found,
			 *   length is < lim - 1
			 */
			do {
				l = getln(s, MAXLINE + 1);
				printf("%s", s);
			} while (s[l - 1] != '\n' && l == MAXLINE);
		}
	return 0;
}
