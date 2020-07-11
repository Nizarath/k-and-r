#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* hash table */

struct node {
	char *macro;
	char *subst;
	struct node *nxt;
};

#define NHASHTAB	101

static struct node *hashtab[NHASHTAB];

unsigned hash(const char *s)
{
	register unsigned h;

	for (h = 0; *s; s++)
		h = 31 * h + *s;
	return h % NHASHTAB;
}

struct node *lookup(const char *macro)
{
	register struct node *np;

	for (np = hashtab[hash(macro)]; np != NULL; np = np->nxt)
		if (!strcmp(np->macro, macro))
			return np;
	return NULL;
}

struct node *install(const char *macro, const char *subst)
{
	struct node *np;
	unsigned h;

	np = lookup(macro);
	if (np == NULL) {
		np = malloc(sizeof(*np));
		if (np == NULL)
			return NULL;
		if ((np->macro = strdup(macro)) == NULL) {
			free(np);
			return NULL;
		}
		h = hash(macro);
		np->nxt = hashtab[h];
		hashtab[h] = np;
	} else
		free(np->subst);
	if ((np->subst = strdup(subst)) == NULL)
		return NULL;

	return np;
}

int undef(const char *macro)
{
	register struct node **prv, *cur;

	/* cann't use lookup() since it'll
	 * return pointer to node without previous pointer
	 */
	prv = &hashtab[hash(macro)];
	for (cur = *prv; cur != NULL; prv = &(cur->nxt), cur = cur->nxt)
		if (!strcmp(cur->macro, macro)) {
			*prv = cur->nxt;
			free(cur->macro);
			free(cur->subst);
			free(cur);
			return 1;
		}
	return 0;
}

void printtab(void)
{
	register struct node **hp, *np;

	for (hp = hashtab; hp < &hashtab[NHASHTAB]; hp++) {
		if (*hp == NULL)
			continue;
		printf("queue: ");
		for (np = *hp; np != NULL; np = np->nxt)
			printf("(%s; %s) ", np->macro, np->subst);
		putchar('\n');
	}
}

/* getch/ungetch */

#define MAXBUF		1000

static char buf[MAXBUF];
static int bp = 0;

int getch(void)
{
	return bp > 0 ? buf[--bp] : getchar();
}

void ungetch(int c)
{
	if (bp < MAXBUF)
		buf[bp++] = c;
	else
		fprintf(stderr, "ungetch: buf fill\n");
}

/* word */

#define MAXWORD		100

int getword(char *word, int max)
{
	register char *w;
	register int c;

	for (w = word; (w - word) < (max - 1) &&
			isalpha(c = getch()); *w++ = c)
		;
	*w = '\0';

	return c;
}

int main(void)
{
	register int c;
	int iscom, isstr;
	char w[MAXWORD], s[MAXWORD], *p;

	iscom = isstr = 0;
	while ((c = getch()) != EOF) {
		switch (c) {
			case '#':
				if (iscom || isstr){	/* comment or string */
					printf("uyes\n");
					break;
				}
				getword(w, MAXWORD);
				if (!strcmp(w, "define")) {
					/* skip spaces */
					while (isspace(c = getch()))
						;
					ungetch(c);
					if (!(isalpha(c) || c == '_'))
						break;

					/* read macro */
					p = w;
					while (isalnum(c = getch()) || c == '_')
						*p++ = c;
					ungetch(c);
					*p = '\0';

					/* skip spaces */
					while (isspace(c = getch()))
						;
					ungetch(c);

					/* read subst */
					p = s;
					while (!isspace(c = getch()))
						*p++ = c;
					ungetch(c);
					*p = '\0';

					/* install to table */
					install(w, s);

				} else {		/* not define */
					for (p = &w[strlen(w) - 1]; p >= w; p--)
						ungetch(*p);
				}	
				break;
			case '/':
				if ((c = getch()) == '*' && !iscom && !isstr)
					iscom = 1;
				else
					ungetch(c);
				break;
			case '*':
				if ((c = getch()) == '/' && iscom && !isstr)
					iscom = 0;
				else
					ungetch(c);
				break;
			case '\\':
				if ((c = getch()) != '\"')
					ungetch(c);
				break;
			case '\"':
				if (!iscom)
					isstr ^= 1;
				break;
		}
	}

	/* show table */
	printtab();

	return 0;
}
