#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>
#include "../../include/weak_functions.h"

static void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

/* Checks got[0..count-1] against expected, then got[count] == NULL. */
static void	check_split(t_ctx *ctx, const char *label,
	char **got, const char **expected, int expected_count)
{
	char	item[128];
	int		i;

	if (!got)
	{
		report_fail(ctx, label, "an array", "NULL");
		return ;
	}
	i = 0;
	while (i <= expected_count)
	{
		snprintf(item, sizeof(item), "%s[%d]", label, i);
		check_str(ctx, item, i < expected_count ? expected[i] : NULL, got[i]);
		if (!got[i])
			break ;
		i++;
	}
	free_split(got);
}

static void	check_split_empty(t_ctx *ctx, const char *label, char **got)
{
	check_split(ctx, label, got, NULL, 0);
}

void	test_ft_split(t_ctx *ctx)
{
	const char	*exp1[] = {"hello"};
	const char	*exp_abc[] = {"a", "b", "c"};
	const char	*exp2[] = {"one", "two"};

	/* empty string → empty array with just NULL */
	check_split_empty(ctx, "split(\"\",' ')", ft_split("", ' '));

	/* all separators → empty array */
	check_split_empty(ctx, "split(\"   \",' ')", ft_split("   ", ' '));

	/* single word, no sep */
	check_split(ctx, "split(\"hello\",' ')", ft_split("hello", ' '), exp1, 1);

	/* non-space separator */
	check_split(ctx, "split(\"a:b:c\",':')", ft_split("a:b:c", ':'), exp_abc, 3);

	/* leading separator */
	check_split(ctx, "split(\" one two\",' ')", ft_split(" one two", ' '), exp2, 2);

	/* trailing separator */
	check_split(ctx, "split(\"one two \",' ')", ft_split("one two ", ' '), exp2, 2);

	/* multiple consecutive separators */
	check_split(ctx, "split(\"one   two\",' ')", ft_split("one   two", ' '), exp2, 2);

	/*
	** '\0' as separator: '\0' is the string terminator, so it is never
	** encountered inside a valid C string. No separator is found and the
	** whole string is returned as a single word.
	** Buggy inner scan loops written as:
	**   while (s[i] != c)          <- no null-guard
	** exit at the NUL terminator (since '\0' == c), but then the outer
	** loop increments i past the end of the string and reads garbage
	** memory, causing a segfault.
	** The correct form is: while (s[i] && s[i] != c)
	*/
	{
		const char	*exp_nul[] = {"hello"};

		check_split(ctx, "split(\"hello\",'\\0')",
			ft_split("hello", '\0'), exp_nul, 1);
	}
	{
		const char	*exp_nul2[] = {"a b c"};

		check_split(ctx, "split(\"a b c\",'\\0')",
			ft_split("a b c", '\0'), exp_nul2, 1);
	}
	check_split_empty(ctx, "split(\"\",'\\0')", ft_split("", '\0'));
}
