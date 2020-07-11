/*
 * calc
 * (if it's not exercise, I'll put into separate files);
 *
 * see "main skeleton" part for description
 */

#include <stdio.h>

/********************** declarations ***********************/

/* common parsing */
int isdigit(int c);	/* shouldn't be macro: isdigit(c++) etc. will fail */
int isspace(int c);
double atof(const char *s);

/* stack */
#define STACK_MAX	100
void push(double d);
double pop(void);

/* process I/O stream */
#define BUF_MAX		100
int getch(void);
void ungetch(int c);

/* read next semantic sequence */
#define ARG_MAX		200
#define NUM		'0'	/* num type */
int getarg(char *s);

/********************** main skeleton **********************/
/*
 * main algorithm is below
 * (it doesn't use getline(), proceeding *semantical* constructs 
 * one by one instead):
 *
 * while ((new construct was read) not EOF):
 * - if it's number -> place to stack;
 * - if it's operation -> do it;
 * - if it's \n -> get result from stack;
 * - else -> error;
 *
 * calc output is after ">", error is after "!" (showing function sometimes)
 */

int main(void)
{
	char s[ARG_MAX];	/* arg sequence */
	register int type;	/* arg type */
	register int tmp;	/* temp for some ops */

	while ((type = getarg(s)) != EOF) {
		switch (type) {
			case NUM:	/* number -> place to stack */
				push(atof(s));
				break;
			case '+':	/* a b + -> (a + b) */
				push(pop() + pop());
				break;
			case '-':	/* a b - -> (a - b), careful with order */
				tmp = pop();
				push(pop() - tmp);
				break;
			case '*':	/* a b * -> (a * b) */
				push(pop() * pop());
				break;
			case '/':	/* a b / -> (a / b) */
				tmp = pop();
				if (!tmp)	/* / 0 */
					printf("! / 0\n");
				else
					push(pop() / tmp);
				break;
			case '%':	/* a b % -> (a % b) */
				tmp = pop();
				push((int)pop() % (int)tmp);
				break;
			case '\n':	/* show result */
				printf("> %lf\n", pop());
				break;
			default:	/* some unexpected */
				printf("! \"%s\" is unknown\n", s);
		}
	}
	
	return 0;
}

/********************** stack of operands ******************/

double stack[STACK_MAX];
int sp = 0;		/* next FREE position */

void push(double d)
{
	if (sp < STACK_MAX)
		stack[sp++] = d;
	else
		printf("! push: stack is full\n");
}

double pop(void)
{
	if (sp > 0)
		return stack[--sp];
	printf("! pop: stack is empty\n");

	return 0.0;
}

/********************** I/O stream processing **************/

char io_buf[BUF_MAX];
int bp = 0;		/* next FREE position */

int getch(void)
{
	return (bp > 0) ? io_buf[--bp] : getchar();
}

void ungetch(int c)
{
	if (bp < BUF_MAX)
		io_buf[bp++] = c;
	else
		printf("! ungetch: io buf is full\n");
}

/*********************** get semantic sequence *************/

int getarg(char *s)
{
	register int c, isfrac;

	/* skip empty chars */
	while (isspace(c = getch()))
		;

	/* first non-empty -- check it:
	 * it's ok for it to be:
	 * - +-*%/ -- operations (after '-' must be not digit)
	 * - 0-9 -- digit, also '-' can be before
	 * - . -- fraction flag is set, next . will be ignored
	 */

	if (!(isdigit(c) || c == '.' || c == '-'))
		return c;		/* first case -> ret op */

	/* not op */
	isfrac = 0;
	*s++ = c;	/* store first (even if it's not needed -- will be
			   overwritten in next call with same s */

	/* special check for minus */
	if (c == '-') {
		c = getch();
		if (!isdigit(c)) {	/* operation, not negative num */
			ungetch(c);
			return '-';
		}
		/* if we here -- then negative num */
		*s++ = c;		/* add to stored - */

	/* also may be empty integer part */
	} else if (c == '.')
		isfrac = 1;

	/* accum all next digits */
	while (isdigit(c = getch()))
		*s++ = c;
	
	/* first non-digit is '.' and there wasn't '.' before -- 
	 * accum fraction part */
	if (c == '.' && !isfrac) {
		*s++ = '.'; 
		while (isdigit(c = getch()))
			*s++ = c;
	}

	/* other case (either non-'.' or already had '.') --
	 * return to buffer */
	*s = '\0';
	if (c != EOF)	/* buffer is chars, cann't store EOF */
		ungetch(c);

	return NUM;
}

/************************ common parsing *******************/

int isdigit(c)
{
	return c >= '0' && c <= '9';
}

int isspace(c)
{
	return c == ' ' || c == '\t';
}

int pow(int x, int y)
{
	register int p;

	for (p = 1; y--; p *= x)
		;
	return p;
}

double atof(const char *s)
{
	double d, p;
	int sign, exp, exp_sign;

	/* skip spaces */
	while (isspace(*s))
		s++;

	/* check sign */
	sign = 1;
	if (*s == '-') {
		sign = -1;
		s++;
	} else if (*s == '+')
		s++;

	/* main loop */
	for (d = 0.0; isdigit(*s); s++)
		d = 10 * d + *s - '0';

	/* check . */
	p = 1.0;	/* here for correct end */
	if (*s == '.') {
		for (s++; isdigit(*s); s++) {
			d = 10 * d + *s - '0';
			p *= 10;
		}
	}

	/* check exp, independently of . */
	if (*s == 'e' || *s == 'E') {
		s++;

		/* get exponent sign */
		exp_sign = 1;
		if (*s == '-') {
			exp_sign = -1;
			s++;
		} else if (*s == '+')
			s++;

		/* accum exp (cann't call atoi -- since
		 * cann't skip spaces etc. */
		for (exp = 0; isdigit(*s); s++)
			exp = 10 * exp + *s - '0';

		/* calc 10^exp */
		exp = pow(10, exp);
		d = (exp_sign == 1) ? (d * exp) : (d / exp);
	}

	/* res = f(d, sign, pow); */
	return sign * (double)d / p;
}

