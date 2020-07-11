#include <stdio.h>
#include <stdarg.h>

int minprintf(const char *fmt, ...)
{
	va_list argp;
	int argc;
	char *s;

	for (va_start(argp, fmt), argc = 0; *fmt; fmt++) {
		if (*fmt != '%')
			putchar(*fmt);
		else
			switch (*++fmt) {
			case 'd':
			case 'i':
				printf("%d", va_arg(argp, int));
				argc++;
				break;
			case 'u':
				printf("%u", va_arg(argp, unsigned));
				argc++;
				break;
			case 'o':
				printf("%o", va_arg(argp, unsigned));
				argc++;
				break;
			case 'x':
			case 'X':
				printf("%x", va_arg(argp, unsigned));
				argc++;
				break;
			case 's':
				for (s = va_arg(argp, char *); *s; s++)
					putchar(*s);
				argc++;
				break;
			case 'f':
				printf("%f", va_arg(argp, double));
				argc++;
				break;
			case 'e':
			case 'E':
				argc++;
				printf("%e", va_arg(argp, double));
				break;
			case 'p':
				argc++;
				printf("%p", va_arg(argp, void *));
				break;
			case '%':
			default:
				putchar(*fmt);
		}
	}
	va_end(argp);

	return argc;
}

#define NARR(arr)	(sizeof (arr) / sizeof((arr)[0]))

int main(void)
{
	int arr[] = {
		minprintf("Cool Lady"),
		minprintf("Cool Lady %s", "Ayrinus\n"),
		minprintf("Cool Lady %s\n", "Ayrinus"),
		minprintf("Cool Lady %s\n", "Ayrinus\n"),
		minprintf("List of %d, %d, %u\n", 12, -12, 12U),
		minprintf("Now float: %f\n", 12e2)
	}, i;

	printf("arr size:\n");
	for (i = 0; i < NARR(arr); i++)
		printf("%d ", arr[i]);
	putchar('\n');

	return 0;
}
