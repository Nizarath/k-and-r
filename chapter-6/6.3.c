#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* list of linenos */
struct lnode {
	unsigned lineno;
	struct lnode *nxt;
};

/* tree */
struct tnode {
	char *word;
	struct lnode *head;
	struct lnode *tail;
	struct tnode *l;
	struct tnode *r;
};

struct tnode *add_tree(struct tnode *tnode, const char *word,
		unsigned lineno)
{
	struct lnode *lnode;
	int cmp;

	if (tnode == NULL) {
		tnode = malloc(sizeof(*tnode));
		if (tnode == NULL)
			goto err;
		tnode->word = strdup(word);
		if (tnode->word == NULL) {
			free(tnode);
			goto err;
		}
		lnode = malloc(sizeof(*lnode));
		if (lnode == NULL) {
			free(tnode->word);
			free(tnode);
			goto err;
		}
		lnode->nxt = NULL, lnode->lineno = lineno;
		tnode->head = tnode->tail = lnode;
		tnode->l = tnode->r = NULL;
	} else {
		cmp = strcmp(tnode->word, word);
		if (cmp < 0)
			tnode->r = add_tree(tnode->r, word, lineno);
		else if (cmp > 0)
			tnode->l = add_tree(tnode->l, word, lineno);
		else if (lineno != tnode->tail->lineno) {
			lnode = malloc(sizeof(*lnode));
			if (lnode == NULL)
				goto err;
			lnode->nxt = NULL, lnode->lineno = lineno;
			tnode->tail->nxt = lnode;
			tnode->tail = lnode;
		}
	}

	return tnode;

err:
	fprintf(stderr, "error %s %d\n", word, lineno);
	exit(1);
}
		
void print_tree(struct tnode *tnode)
{
	register struct lnode *lp;

	if (tnode != NULL) {
		print_tree(tnode->l);

		/* print */
		printf("%s: ", tnode->word);
		for (lp = tnode->head; lp != NULL; lp = lp->nxt)
			printf("%d ", lp->lineno);
		putchar('\n');

		print_tree(tnode->r);
	}
}

/* getword part */
#define MAXWORD		100

/* word is written,
 * first incorrect char is returned */
int getword(char *word, int max)
{
	register char *w;
	register int c;

	for (w = word; (w - word) < (max - 1) &&
			isalpha(c = getchar()); *w++ = c)
		;
	*w = '\0';

	return c;
}

/* main work */
int main(void)
{
	char word[MAXWORD];
	struct tnode *root;
	register unsigned lineno;
	register int c;

	/* read and place */
	for (root = NULL, lineno = 1; (c = getword(word, MAXWORD)) != EOF; ) {
		if (isalpha(*word))
			root = add_tree(root, word, lineno);
		if (c == '\n')
			lineno++;
	}

	/* show */
	print_tree(root);
	return 0;
}

