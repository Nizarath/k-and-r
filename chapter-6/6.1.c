/* some tests: _auto, #auto, auto_, char
 * char char auto auto auto*/
#include <stdio.h>
#include <stdlib.h>

/* getch/ungetch */
#define MAXBUF		100
static char buf[MAXBUF];
static int bp = 0;

int getch(void)
{
	return bp > 0 ? buf[--bp] : getchar();
}

void ungetch(int c)
{
	if (bp >= MAXBUF)
		printf("ungetch: buf is fill\n");
	else
		buf[bp++] = c;
}

/* check commentaries and strings */
static int iscom = 0;
static int isstr = 0;

/* getword: read the words */

#define MAXWORD		100

int getword(char *word, int lim)
{
	int c;
	char *w;

	w = word;
	while (isspace(c = getch()))
		;
	if (c == EOF)
		return EOF;
	

	/* inside comments -- immediatly finish if not the end of comment */
	if (iscom) {
		if (c == '*') {
			if ((c = getch()) == '/')
				iscom = 0;
			else
				ungetch(c);
		}
		return *word = '\0';
	}

	/* inside string literal -- immediatly finish if not the end of it */
	if (isstr) {
		if (c == '\\')
			getch();	/* skip possible \" */
		else if (c == '\"')	/* end of pars */
			isstr = 0;
		return *word = '\0';
	}

	/* process input */
	for (ungetch(c); --lim > 0; w++) {
		/* keyword is separated in the end -- normal case, count it */
		if (isspace(*w = getch()) || *w == '\n' || *w == '(' || 
				*w == ',' || *w == ';' || *w == ':')
			break;
		if (*w == '_')		/* not keyword, but name */
			return *word = '\0';
		if (*w == '/') {	/* check commentaries */
			if ((c = getch()) == '*') {
				iscom = 1;
			} else
				ungetch(c);
			return *word = '\0';
		}
		if (*w == '\\')		/* check brackets */
			getch();	/* skip possible \" */
		else if (*w == '\"') {
			isstr = 1;
			return *word = '\0';
		}
	}
	*w = '\0';
	return *word;
}

/* key words of C */

struct key {
	char *word;
	int cnt;
} keytab[] = {
	{ "auto", 0 },
	{ "break", 0 },
	{ "case", 0 },
	{ "char", 0 },
	{ "const", 0 },
	{ "continue", 0 },
	{ "default", 0 }
};

#define NKEYS	(sizeof keytab / sizeof(struct key))

/* comparator */
int keycmp(const void *key, const void *rec)
{
	return strcmp((const char *)key, ((struct key *)rec)->word);
}

int main(int argc, char *argv[])
{
	auto struct key *keyp;
	char word[MAXWORD];
	/* test this: auto, char */
	int _auto;

	/* read new word */
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(*word))	/* keyword */
			/* find and inc freq */
			if ((keyp = bsearch(word, keytab, NKEYS, sizeof(struct key),
							keycmp)) != NULL)
				keyp->cnt++;

	/* show all */
	for (keyp = keytab; keyp < &keytab[NKEYS]; keyp++)
		if (keyp->cnt)
			printf("%s: %d\n", keyp->word, keyp->cnt);
	return 0;
}
