/*
 * word length histogram
 */

#include <stdio.h>

#define MAXLEN	30
#define FRQSCALE 24 /* scale to show on one line of screen */

/* inside/outside word */
#define INW	1
#define OUTW	0

int main(void)
{
	int freq[MAXLEN], c, cnt, state, maxfreq;
	double k;

	/* initialization */
	for (state = 0; state < MAXLEN; state++)
		freq[state] = 0;
	state = OUTW, cnt = 0;

	/* calculate length */
	while ((c = getchar()) != EOF)
		if (c == ' ' || c == '\t' || c == '\n') {
			if (state == INW) {
				freq[cnt - 1]++;
				cnt = 0;
			}
			state = OUTW;
		} else {
			state = INW;
			cnt++;
		}
	/* choose biggest frequency to have scale */
	for (maxfreq = c = 0; c < MAXLEN; c++)
		if (freq[c] > maxfreq)
			maxfreq = freq[c];
	/* scale all */
	k = (double) FRQSCALE / maxfreq;
	for (c = 0; c < MAXLEN; c++)
		freq[c] *= k;
	/* show results */
	for (c = FRQSCALE; c > 0; c--) {
		printf("%d\t| ", c);
		for (cnt = 0; cnt < MAXLEN; cnt++)
			printf("%c ", (freq[cnt] < c) ? ' ' : '*');
		putchar('\n');
	}
	return 0;
}
