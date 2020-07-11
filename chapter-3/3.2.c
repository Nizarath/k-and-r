/*
 * escape(s, t) -- 
 * replace all \n from t with \\n
 */

#include <stdio.h>

#define MAXLINE		80

/* getline */
int getln(char *s, int max)
{
	register char *b;
	register int c;

	for (b = s; (s - b) < (max - 1) &&
		(c = getchar()) != EOF; *s++ = c)
		if (c == '\n') {
			*s++ = c;
			break;
		}
	*s = '\0';
	
	return s - b;
}

/*
 * t -> s
 * with replacing of \n symbols
 */
void escape(char *s, const char *t)
{
	while (*t) {
		switch (*t) {
			case '\t':
				*s++ = '\\';
				*s++ = 't';
				break;
			case '\n':
				*s++ = '\\';
				*s++ = 'n';
				break;
			default:
				*s++ = *t;
		}
		t++;
	}
	/* finish string */
	*s = '\0';
}

/*
 * t -> s
 * with replacing to \n symbols
 */
void unescape(char *s, const char *t)
{
	while (*t) {
		if (*t == '\\') {
			t++;
			switch (*t) {
				case 'n': *s++ = '\n'; break;
				case 't': *s++ = '\t'; break;
				default:
					  *s++ = '\\';
					  *s++ = *t;
			}
		} else
			*s++ = *t;
		t++;
	}
	*s = '\0';
}

/* testing */
int main(void)
{
	char s[MAXLINE], t[MAXLINE];

	while (getln(t, MAXLINE) > 0) {
		unescape(s, t);
		printf("> %s", s);
	}
	return 0;
}
