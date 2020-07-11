#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tree of var names */
struct tnode {
	char *name;
	struct tnode *l;
	struct tnode *r;
};

static int nvarchars = 6;

struct tnode *add_tnode(struct tnode *node, char *name)
{
	int c;

	if (node == NULL) {
		/* alloc */
		node = malloc(sizeof(struct tnode));
		if (node == NULL) {		/* node itself */
			fprintf(stderr, "error alloc node for %s\n", name);
			exit(1);
		}
		node->name = strndup(name, nvarchars);
		if (node->name == NULL) {	/* name */
			free(node);
			fprintf(stderr, "error alloc string for %s\n", name);
			exit(1);
		}
		/* links */
		node->l = node->r = NULL;
	} else {
		c = strncmp(name, node->name, nvarchars);
		if (c < 0)
			node->l = add_tnode(node->l, name);
		else if (c > 0)
			node->r = add_tnode(node->r, name);
	}

	return node;
}

void treeprint(struct tnode *root)
{
	if (root != NULL) {
		treeprint(root->l);
		printf("%s\n", root->name);
		treeprint(root->r);
	}
}

/* word-related stuff */
#define MAXWORD		100

int issep(int c)
{
	return c == '(' || c == ')' || c == ';' || c == ':';
}

int getword(char *w, int max)
{
	register char *b;
	register int c;

	for (b = w; (w - b) < (max - 1) && (c = getchar()) != '\n' 
			&& !isspace(c) && !issep(c); *w++ = c)
		if (c == EOF) {
			*w = '\0';
			return EOF;
		}
	*w = '\0';

	return *b;
}

/* check if word is variable in C program part */

static int iscom = 0;		/* in commentary */
static int isstr = 0;		/* in string literal */

static char *keys[] = {
	"auto", "break", "case",
	"char", "const", "continue",
	"default", "do", "double",
	"else", "enum", "extern",
	"float", "for", "goto",
	"if", "int", "long",
	"register", "return", "short",
	"signed", "sizeof", "static",
	"struct", "switch", "typedef",
	"union", "unsigned", "void",
	"volatile", "while"
};

#define NKEYS	(sizeof keys / sizeof(keys[0]))

int scmp(const void *name, const void *keyp)
{
	return strcmp((const char *)name, *(const char **)keyp);
}

int isvar(char *word)
{
	register char *w;

	/* [a-zA-Z][0-9a-zA-Z]*, not a keyword */
	if (!isalpha(*word) || bsearch(word, keys, NKEYS, sizeof(char *), scmp)
			!= NULL)
		return 0;
	for (w = word + 1; *w; w++)
		if (!isalnum(*w))
			return 0;

	/* not in commentaries nor in brackets */
	return !(iscom || isstr);
}

/* check for commentaries or " */
int parse_word(const char *w)
{
	for (; *w; w++)
		if (*w == '/') {
			if (*++w == '*' && !isstr)
				iscom = 1;
			else
				w--;
		} else if (*w == '*') {
			if (*++w == '/' && !isstr)
				iscom = 0;
			else
				w--;
		} else if (*w == '\\')
			w++;	/* skip possible \" */
		else if (*w == '\"' && !iscom)
			isstr ^= 1;
}


int main(int argc, char *argv[])
{
	struct tnode *root;
	char word[MAXWORD];

	if (argc > 1)
		nvarchars = atoi(*++argv);

	/* read and place */
	for (root = NULL; getword(word, MAXWORD) != EOF; ) {
		parse_word(word);	/* switch comment modes if needed */
		if (isvar(word) && !iscom && !isstr)
			root = add_tnode(root, word);
	}

	/* show words */
	printf("Tree is:\n");
	treeprint(root);

	return 0;
}
