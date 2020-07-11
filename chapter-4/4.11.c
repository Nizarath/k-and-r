/*
 * calc
 * (if it's not exercise, I'll put into separate files);
 *
 * see "main skeleton" part for description
 * (it's math version, for gcc to compile and link use
 * $ cc <file>.c -lm
 */

#include <stdio.h>
#include <math.h>

/********************** declarations ***********************/

/* common parsing */
int isdigit(int c);	/* shouldn't be macro: isdigit(c++) etc. will fail */
int isspace(int c);
int islalpha(int c);
double atof(const char *s);

/* stack */
#define STACK_MAX	100
void push(double d);
double pop(void);
void clear_stack(void);

/* process I/O stream */
int getch(int *buf);

/* read next semantic sequence */
#define ARG_MAX		200
#define NUM		'0'	/* num type */
#define CMD		'1'	/* command type */
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
	register double tmp, tmp2; /* temp for some ops */
	char *cmd_table[] = {
		"p",		/* print top of stack, not remove */
		"d",		/* dublicate top */
		"s",		/* swap two tops */
		"c",		/* clear all stack */
		"sin",		/* sin(x) */
		"cos",		/* cos(x) */
		"exp",		/* exp(x) */
		"pow",		/* pow(x,y) */
		"set"		/* set var */
	};
	double vars[26];	/* for a-z */

	while ((type = getarg(s)) != EOF) {
		switch (type) {
			case CMD:	/* command -> find in table and execute */

				/* here is very stupid brute force,
				 * should be redone to binsearch, then calling
				 * a function pointer in a table */

				if (!strcmp(s, "p")) {		/* print top */
					tmp = pop();
					printf("> %lf\n", tmp);
					push(tmp);
				} else if (!strcmp(s, "d")) {	/* dublicate top */
					tmp = pop();
					push(tmp);
					push(tmp);
				} else if (!strcmp(s, "s")) {	/* swap two tops */
					tmp = pop();
					tmp2 = pop();
					push(tmp);
					push(tmp2);
				} else if (!strcmp(s, "c")) /* clear stack */
					clear_stack();
				else if (!strcmp(s, "sin")) /* sin(x) */
					push(sin(pop()));			
				else if (!strcmp(s, "cos")) /* cos(x) */
					push(cos(pop()));
				else if (!strcmp(s, "exp")) /* exp(x) */
					push(exp(pop()));
				else if (!strcmp(s, "pow")) { /* pow(x,y) */
					tmp = pop();
					push(pow(pop(), tmp));
				} else
					printf("! unknown command %s\n", s);
				break;
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

void clear_stack(void)
{
	sp = 0;
}

/*********************** get semantic sequence *************/

int getch(int *buf)
{
	int c;

	/* buf is empty -- read stdin */
	if (*buf == 0)
		return getchar();

	/* buf isn't empty */
	c = *buf, *buf = 0;
	return c;
}

int getarg(char *s)
{
	register int c, isfrac;
	static int b = 0;

	/* skip empty chars */
	while (isspace(c = getch(&b)))
		;

	/* first non-empty -- check it:
	 * it's ok for it to be:
	 * - +-*%/ -- operations (after '-' must be not digit)
	 * - 0-9 -- digit, also '-' can be before
	 * - . -- fraction flag is set, next . will be ignored
	 * - a-z -- command start
	 */

	if (!(isdigit(c) || c == '.' || c == '-' || islalpha(c)))
		return c;		/* first case -> ret op */

	/* check for command */
	if (islalpha(c)) {
		/* accum it, including current */
		do {
			*s++ = c;
		} while (islalpha(c = getch(&b)));
		*s = '\0';

		/* return last to stream */
		b = c;
		return CMD;		/* command type */
	}

	/* will be number */
	isfrac = 0;

	/* special check for minus */
	if (c == '-') {
		*s++ = c;
		c = getch(&b);
		if (!isdigit(c)) {	/* operation, not negative num */
			b = c;
			return '-';
		}
		/* if we here -- then negative num */
		*s++ = c;		/* add to stored - */

	/* also may be empty integer part */
	} else if (c == '.') {
		*s++ = '.';
		isfrac = 1;
	} else		/* digit */
		*s++ = c;

	/* accum all next digits */
	while (isdigit(c = getch(&b)))
		*s++ = c;
	
	/* first non-digit is '.' and there wasn't '.' before -- 
	 * accum fraction part */
	if (c == '.' && !isfrac) {
		*s++ = '.'; 
		while (isdigit(c = getch(&b)))
			*s++ = c;
	}

	/* other case (either non-'.' or already had '.') --
	 * return to bfer */
	*s = '\0';
	b = c;

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

int islalpha(c)
{
	return c >= 'a' && c <= 'z';
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

/*********************** string routines ************************/

int strcmp(const char *s0, const char *s1)
{
	for (; *s0 == *s1; s0++, s1++)
		if (*s0 == '\0')
			return 0;
	return *s0 - *s1;
}

