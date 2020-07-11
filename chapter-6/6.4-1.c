/*
 * error checks are skipped
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tree with words */
struct tnode {
	char *word;
	int frq;
	struct tnode *l, *r;
};

struct tnode *add_tree(struct tnode *root, char *word)
{
	int cmp;

	if (root == NULL) {
		root = malloc(sizeof(*root));
		root->word = strdup(word);
		root->l = root->r = NULL;
		root->frq = 1;
	} else {
		cmp = strcmp(word, root->word);
		if (cmp < 0)
			root->l = add_tree(root->l, word);
		else if (cmp > 0)
			root->r = add_tree(root->r, word);
		else
			root->frq++;
	}
	return root;
}

/* tree with frequencies */
struct lnode {
	char *word;
	struct lnode *nxt;
};

struct tnode2 {
	int frq;
	struct lnode *head, *tail;
	struct tnode2 *l, *r;
};

struct tnode2 *add_tree2(struct tnode2 *root, char *word, int frq)
{
	struct lnode *lnode;

	if (root == NULL) {
		root = malloc(sizeof(*root));
		root->frq = frq;
		root->l = root->r = NULL;
		lnode = malloc(sizeof(*lnode));
		lnode->word = strdup(word);
		lnode->nxt = NULL;
		root->head = root->tail = lnode;
	} else {
		if (root->frq > frq)
			root->l = add_tree2(root->l, word, frq);
		else if (root->frq < frq)
			root->r = add_tree2(root->r, word, frq);
		else {
			lnode = malloc(sizeof(*lnode));
			lnode->word = strdup(word);
			lnode->nxt = NULL;
			root->tail->nxt = lnode;
			root->tail = lnode;
		}
	}
	return root;
}

/* tr1 -> tr2 */
void tr1tr2(struct tnode *root, struct tnode2 **root2)
{
	if (root != NULL) {
		tr1tr2(root->r, root2);
		*root2 = add_tree2(*root2, root->word, root->frq);
		tr1tr2(root->l, root2);
	}
}

/* print tr1 */
void prtr(struct tnode *root)
{
	if (root != NULL) {
		prtr(root->l);
		printf("%s: %d\n", root->word, root->frq);
		prtr(root->r);
	}
}

/* print tr2 */
void prtr2(struct tnode2 *root)
{
	register struct lnode *lp;

	if (root != NULL) {
		prtr2(root->r);
		printf("%d: ", root->frq);
		for (lp = root->head; lp != NULL; lp = lp->nxt)
			printf("%s ", lp->word);
		putchar('\n');
		prtr2(root->l);
	}
}

/* getword */

#define MAXWORD		100

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

int main(void)
{
	char word[MAXWORD];
	struct tnode *root;
	struct tnode2 *root2;

	root = (struct tnode *)(root2 = NULL);
	/* calc freqs */
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(*word))
			root = add_tree(root, word);

	/* create the freq tree */
	tr1tr2(root, &root2);

	/* show */
	prtr2(root2);

	return 0;
}
