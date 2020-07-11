/*
 * dcl program,
 * fixed:
 * - int arr[f f f f f]
 * - [] fun()
 * - clearing all after any errors
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* getch/ungetch */
#define	MAXBUF		1000
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

/* main work */
#define MAXTOKEN	100
enum { NAME, PARENS, BRACKETS };
void dcl(void);
void dirdcl(void);
void undcl(void);

/* global vars */
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

/* error occured, clean all state */
void error(const char *msg)
{
	fprintf(stderr, msg);
	tokentype = 0;
	*token = *name = *datatype = *out = '\0';
	main();		/* restart */
}

int gettoken(void)
{
	register int c;
	register char *p;

	p = token;
	while ((c = getch()) == ' ' || c == '\t')	/* skip spaces */
		;
	if (c == '(') {
		if ((c = getch()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = c; (*p = getch()) != ']'; p++)
			if (!isalnum(*p))
				error("array size isn't var/num\n");
		*++p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getch()); )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	} else
		return tokentype = c;
}

/*
 * undcl:
 * english (modified) -> C, e.g.:
 * x [] * () int ->  int (*x[])()
 */
int main(void)
{
	int type;
	char temp[MAXTOKEN];

	while (gettoken() != EOF) {
		strcpy(out, token);
		while ((type = gettoken()) != '\n')
			if (type == BRACKETS || type == PARENS) {
				if (*out == '*') {
					strcpy(temp, "(");
					strcat(temp, out);
					strcat(temp, ")");
					strcpy(out, temp);
				}
				strcat(out, token);
			} else if (type == '*') {
				strcpy(temp, "*");
				strcat(temp, out);
				strcpy(out, temp);
			} else if (type == NAME) {
				strcpy(temp, token);
				strcat(temp, " ");
				strcat(temp, out);
				strcpy(out, temp);
			} else
				error("invalid input\n");
		printf("%s\n", out);
	}
	return 0;
}

int istype(char *s)
{
	if (!isalpha(*s))
		return 0;
	while (!isspace(*s) && *s) {
		if (!isalnum(*s))
			return 0;
		s++;
	}
	return 1;
}
