/*
 * day_of_year, month_day
 */

#include <stdio.h>

static char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* (year; month; day of month) -> day of year */
int day_of_year(int year, int month, int day)
{
	int i, leap;

	/* error check 1 -- monthes */
	if (month < 1 || month > 12)
		return -1;

	/* is year leap */
	leap = (!(year % 4) && (year % 100)) || !(year % 400);

	/* error check 2 -- day */
	if (day < 1 || day > daytab[leap][month])
		return -1;

	/* calc */
	for (i = 1; i < month; i++)
		day += daytab[leap][i];

	return day;
}

/* (year; day of year) -> (month; day of month) */
int month_day(int year, int yearday, int *pmonth, int *pday)
{
	int i, leap;

	leap = (!(year % 4) && (year % 100)) || !(year % 400);

	/* error check -- yearday */
	if (yearday < 1 || yearday > (leap ? 366 : 365))
		return -1;

	/* calc */
	for (i = 1; yearday > daytab[leap][i]; i++)
		yearday -= daytab[leap][i];
	*pmonth = i;
	*pday = yearday;

	return 0;
}

/* test */
int main(void)
{
	int d, y, m, yd, res;

	while (1) {	/* ^C to end */
		/* (y; m; d) -> yd */
		printf("(y; m; d): ");
		scanf("%d %d %d", &y, &m, &d);
		res = day_of_year(y, m, d);
		if (res == -1)
			printf("> incorrect input\n");
		else
			printf("> %d\n", res);
		/* (y; yd) -> (m; d) */
		printf("(y; yd): ");
		scanf("%d %d", &y, &yd);
		if (month_day(y, yd, &m, &d) == -1)
			printf("> incorrect input\n");
		else
			printf("> (%d; %d)\n\n", m, d);
	}

	return 0;
}
