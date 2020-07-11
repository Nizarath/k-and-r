#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* hash table */

struct node {
	char *macro;
	char *subst;
	struct node *nxt;
};

#define NHASHTAB	2	/* make this 101 or some prime big num for real
				   cases */

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
		printf("queue: ");
		for (np = *hp; np != NULL; np = np->nxt)
			printf("(%s; %s) ", np->macro, np->subst);
		putchar('\n');
	}
}

/* getline part */

#define MAXLINE		1000

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

int main(void)
{
	install("pi", "3.1415");
	install("ex", "2.718281828");
	install("q", "1234.5");
	install("tt", "56456.7");
	install("tr", "12555");
	undef("tytyt");
	undef("tt");
	undef("tr");
	undef("ex");
	undef("q");
	undef("pi");
	printtab();
	return 0;
}
