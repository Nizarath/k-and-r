#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

int minscanf(const char *fmt, ...)
{
	va_list argp;
	int argc;

	for (argc = 0, va_start(argp, fmt); *fmt; fmt++) {
		if (isblank(*fmt))
			continue;
		if (*fmt == '%')
			switch (*++fmt) {
				case 'd':
				case 'i':
					argc++;
					scanf("%d", va_arg(argp, int *));
					continue;
				case 'o':
					argc++;
					scanf("%o", va_arg(argp, unsigned *));
					continue;
				case 'u':
					argc++;
					scanf("%d", va_arg(argp, unsigned *));
					continue;
				case 'x':
					argc++;
					scanf("%x", va_arg(argp, unsigned *));
					continue;
				case 'c':
					argc++;
					scanf("%c", va_arg(argp, char *));
					continue;
				case 's':
					argc++;
					scanf("%s", va_arg(argp, char *));
					continue;
				case 'e':
					argc++;
					scanf("%lf", va_arg(argp, double *));
					continue;
				case 'f':
					argc++;
					scanf("%f", va_arg(argp, float *));
					continue;
				/* including %% */
			}
		if (getchar() != *fmt)
			break;
	}

	return argc;

}

int main(void)
{
	int a, b, c, cnt;

	a = b = c = 0;
	cnt = minscanf("%d %d %d", &a, &b, &c);
	printf("a is %d\nb is %d\nc is %d\n", a, b, c);
	printf("cnt is %d\n", cnt);

	return 0;
}

