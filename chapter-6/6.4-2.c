/*
 * error checks are skipped
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* count */
static int el_cnt = 0;

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
		el_cnt++;
	} else {
		cmp = strcmp(word, root->word);
		if (cmp < 0)
			root->l = add_tree(root->l, word);
		else if (cmp > 0)
			root->r = add_tree(root->r, word);
		else {
			root->frq++;
			el_cnt++;
		}
	}
	return root;
}

/* array with frequencies */

struct el {
	char *word;
	int frq;
};

static int top = 0;

/* fill the array */
void tr2arr(struct tnode *root, struct el *arr)
{
	if (root != NULL) {
		tr2arr(root->l, arr);
		arr[top].word = strdup(root->word);
		arr[top].frq = root->frq;
		top++;
		tr2arr(root->r, arr);
	}
}

/* comparator */
int elcmp(const void *el1, const void *el2)
{
	return ((struct el*)el1)->frq < ((struct el*)el2)->frq;
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
	struct el *arr;

	/* calc freqs */
	root = NULL;
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(*word))
			root = add_tree(root, word);

	/* arr with freqs */
	arr = malloc(sizeof(*arr) * el_cnt);

	/* fill it */
	tr2arr(root, arr);

	/* sort it */
	qsort(arr, top, sizeof(struct el), elcmp);

	/* show arr */
	while (top--) {
		printf("%d %s\n", arr->frq, arr->word);
		arr++;
	}

	return 0;
}
