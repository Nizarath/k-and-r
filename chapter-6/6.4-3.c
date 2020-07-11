/*
 * error checks are skipped
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tree with words */
typedef struct tnode {
	const char *word;
	int frq;
	struct tnode *l, *r;
} tnode;

tnode *add_tree(tnode *root, const char *word)
{
	int cmp;

	if (root == NULL) {
		root = malloc(sizeof(*root));
		if (root == NULL)
			return NULL;
		if ((root->word = strdup(word)) == NULL) {
			free(root);
			return NULL;
		}
		root->frq = 1;
		root->l = root->r = NULL;
	} else {
		cmp = strcmp(root->word, word);
		if (cmp < 0)
			root->r = add_tree(root->r, word);
		else if (cmp > 0)
			root->l = add_tree(root->l, word);
		else
			root->frq++;
	}

	return root;
}

/* list with frequencies */
typedef struct lnode {
	const char *word;
	int frq;
	struct lnode *nxt;
} lnode;

/* add to list;
 * word IS NOT copied, since it's one to one with
 * tree's word
 */
void add_ls(struct lnode **head, const char *word, int frq)
{
	struct lnode *nd, **np;

	/* alloc */
	nd = malloc(sizeof(*nd));
	if (nd == NULL)
		return;
	nd->word = word;
	nd->frq = frq;

	/* loop in list */
	for (np = head; *np != NULL && frq <= (*np)->frq; np = &((*np)->nxt))
		;
	nd->nxt = *np;
	*np = nd;
}

/* tree -> list,
 * free tree node exactly after
 */
void tr2ls(struct tnode *root, struct lnode **head)
{
	if (root != NULL) {
		tr2ls(root->r, head);
		add_ls(head, root->word, root->frq);
		tr2ls(root->l, head);
		free(root);
	}
}

/* read words */

#define MAXWORD		100

int getword(char *s, int max)
{
	register char *w;
	register int c;

	for (w = s; (s - w) < (max - 1) &&
			isalpha(c = getchar()); *s++ = c)
		;
	*s = '\0';

	return c;
}

/* show list and free nodes in parallel */
void prls(struct lnode *head)
{
	register lnode *t;

	for (; head != NULL; head = t) {
		t = head->nxt;
		printf("%d %s\n", head->frq, head->word);
		free((void *)head->word);
		free(head);
	}
}

/* test all */
int main(void)
{
	struct tnode *root;
	struct lnode *head;
	char w[MAXWORD];

	/* read words */
	for (root = NULL, head = NULL; getword(w, MAXWORD) != EOF; )
		if (isalpha(*w))
			root = add_tree(root, w);
	tr2ls(root, &head);	/* tree -> list */
	prls(head);		/* show list */
	return 0;
}
