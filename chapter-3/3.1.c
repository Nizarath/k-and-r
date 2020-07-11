/*
 * testing 2 bsearch versions
 */

#include <stdio.h>
#include <stdlib.h>	/* rand() for tests */

/* classical bsearch */
int bsearch1(int *arr, int n, int x)
{
	register int l, m, h;

	l = 0, h = n - 1;
	while (l <= h) {
		m = (l + h) >> 1;
		if (arr[m] > x)	/* lower part */
			h = m - 1;
		else if (arr[m] < x)	/* upper part */
			l = m + 1;
		else	/* match */
			return m;
	}
	/* not found */
	return -1;
}

/*
 * updated version
 *
 * hint: it's the only correct, 
 * version with if (arr[m] <= x) l = m;
 * etc. will loop forever where there are 2 el-s:
 *
 * 1. Initial:
 * | x | x |
 * ^   ^
 * l   h
 * 
 * 2. Calc m:
 * m = (l + h) / 2 = 0;
 * | x | x |
 * ^   ^
 * l,m h
 *
 * 3. l = m -- the same, return to 1
 *
 * The reason is way of rounding.
 * So only if (arr[m] >= x) h = m; is correct way to do it.
 */
int bsearch2(int *arr, int n, int x)
{
	register int l, m, h;

	l = 0, h = n - 1;
	while (l < h) {	/* not strict for n == 1 case */
		m = (l + h) >> 1;
		if (arr[m] >= x) /* lower OR this */
			h = m;
		else		/* upper */
			l = m + 1;
	}
	/* check l or h, but not m (for 1 el.) */
	return (arr[l] == x) ? l : -1;
}

/* tests */
#define NARR(a)		(sizeof(a) / sizeof(a[0]))
#define SIZE		2000000

int icmp(const void *ia1, const void *ia2)
{
	return *(int *)ia1 - *(int *)ia2;
}

int main(void)
{
	int arr[SIZE], i;

	/* fill arr */
	for (i = 0; i < SIZE; i++)
		arr[i] = rand() % (SIZE * 3);

	/* sort it */
	qsort(arr, SIZE, sizeof(int), &icmp);

	/* try to find all el, change to bsearch2 here
	 * to see the difference, on SIZE = 2000000
	 * on Linux 4.4.39 on 2.8GHz x86 (32-bit) and 4Gb
	 * it's ~108 ms */
	for (i = 0; i < SIZE * 3; i++)
		bsearch1(arr, SIZE, i);

	return 0;
}
