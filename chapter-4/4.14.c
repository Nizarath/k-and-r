/*
 * macros
 */
#include <stdio.h>

#define swap(t, x, y)	{	\
		t tmp;		\
		tmp = x;	\
		x = y;		\
		y = tmp;	\
	}

/* test */
int main(void)
{
	int x, y;

	x = 12L, y = 0xff;
	swap(int, x, y);
	printf("%d %d\n", x, y);

	struct s {
		int a;
		int b;
	} s1 = { 12, 45 }, s2 = { 0xdcUL, 12L };
	swap(struct s, s1, s2);
	printf("%d %d, %d %d\n", s1.a, s1.b, s2.a, s2.b);
	
	return 0;
}
