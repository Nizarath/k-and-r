/*
 * calc
 * (if it's not exercise, I'll put into separate files);
 *
 * see "main skeleton" part for description
 */

#include <stdio.h>

/********************** declarations ***********************/

/* stack */
#define STACK_MAX	100
void push(double d);
double pop(void);

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
	double d1, d2, tmp;
	char op;

	while (1) {
		if (scanf("%lf %lf %c", &d1, &d2, &op) != 3)
			printf("! some err\n");
		else {
			push(d1);		/* prepare operands */
			push(d2);
			switch (op) {		/* prepare operation */	
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
			default:	/* some unexpected */
				printf("! %c is unknown\n", op);
			}

			printf("> %lf\n", pop());
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
