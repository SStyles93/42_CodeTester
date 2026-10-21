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

static void	check_split(t_ctx *ctx, const char *label,
	char **got, const char **expected, int expected_count)
{
	int	i;

	if (!got)
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → got NULL array\n", label);
		return ;
	}
	i = 0;
	while (i < expected_count)
	{
		if (!got[i])
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET " %s[%d]: expected \"%s\", got NULL\n",
					label, i, expected[i]);
			free_split(got);
			return ;
		}
		if (strcmp(got[i], expected[i]) == 0)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET " %s[%d] = \"%s\"\n",
					label, i, got[i]);
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET " %s[%d]: expected \"%s\", got \"%s\"\n",
					label, i, expected[i], got[i]);
		}
		i++;
	}
	/* NULL sentinel at end */
	if (got[expected_count] == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s: NULL-terminated\n", label);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s: missing NULL terminator\n", label);
	}
	free_split(got);
}

static void	check_split_empty(t_ctx *ctx, const char *label, char **got)
{
	if (got && got[0] == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s → empty array\n", label);
		free(got);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected empty array\n", label);
		free_split(got);
	}
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

	/* '\0' separator: never matches inside the string → whole string */
	check_split(ctx, "split(\"hello\",'\\0')", ft_split("hello", '\0'), exp1, 1);
	check_split_empty(ctx, "split(\"\",'\\0')", ft_split("", '\0'));
}
