/*
 * frequency historgram
 */

#include <stdio.h>
#include <stdlib.h>

/* basic counts */
#define NALPHA		26
#define NDIGIT 		10

/* checking if belongs */
#define ISDIGIT(c)	((c) >= '0' && (c) <= '9')
#define ISALPHAL(c)	((c) >= 'a' && (c) <= 'z')
#define ISALPHAU(c)	((c) >= 'A' && (c) <= 'Z')

int main(void)
{
	/* alphabet, digits, ' ', '\t', '\n' */
	int alpha_low[NALPHA], alpha_up[NALPHA],
		digits[NDIGIT], ns, nl, nt, unk;
	/* symbol to read */
	int c, i;

	/* clear all mem */
	for (c = 0; c < NALPHA; c++)
		alpha_low[c] = alpha_up[c] = 0;
	for (c = 0; c < NDIGIT; c++)
		digits[c] = 0;
	ns = nl = nt = unk = 0;

	/* count symbols */
	while ((c = getchar()) != EOF)
		if (ISDIGIT(c))
			digits[c - '0']++;
		else if (ISALPHAL(c))
			alpha_low[c - 'a']++;
		else if (ISALPHAU(c))
			alpha_up[c - 'A']++;
		else switch (c) {
			case ' ':	ns++;	break;
			case '\t':	nt++;	break; 
			case '\n':	nl++;	break;
			default:	unk++;
		}

	/* create histogram and scale in parallel */
	/* a-z */
	for (c = 0; c < NALPHA; c++) {
		printf("%c: %d ", 'a' + c, alpha_low[c]);
		for (i = 0; i < alpha_low[c]; i++)
			putchar('*');
		putchar('\n');
	}
	/* A-Z */
	for (c = 0; c < NALPHA; c++) {
		printf("%c: %d ", 'A' + c, alpha_up[c]);
		for (i = 0; i < alpha_up[c]; i++)
			putchar('*');
		putchar('\n');
	}
	/* 0-9 */
	for (c = 0; c < NDIGIT; c++) {
		printf("%c: %d ", '0' + c, digits[c]);
		for (i = 0; i < digits[c]; i++)
			putchar('*');
		putchar('\n');
	}
	/* ' ', '\t', '\n' */
	printf(" : %d ", ns);
	for (i = 0; i < ns; i++)
		putchar('*');
	printf("\n\\t: %d ", nt);
	for (i = 0; i < nt; i++)
		putchar('*');
	printf("\n\\n: %d ", nl);
	for (i = 0; i < nl; i++)
		putchar('*');
	putchar('\n');
	return 0;
}
