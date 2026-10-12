#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>


char	**ft_split(char const *s, char c);

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

void	test_ft_split(t_ctx *ctx)
{
	char		**r;
	const char	*exp0[] = {NULL};
	const char	*exp1[] = {"hello", NULL};
	const char	*exp3[] = {"hello", "world", "foo"};
	const char	*exp_abc[] = {"a", "b", "c"};
	const char	*exp2[] = {"one", "two"};

	/* empty string → empty array with just NULL */
	r = ft_split("", ' ');
	if (r && r[0] == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " split(\"\") → empty array\n");
		free(r);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " split(\"\") → expected empty array\n");
		free_split(r);
	}
	(void)exp0;

	/* single word, no sep */
	r = ft_split("hello", ' ');
	check_split(ctx, "split(\"hello\",' ')", r, exp1, 1);

	/* three words */
	r = ft_split("hello world foo", ' ');
	check_split(ctx, "split(\"hello world foo\",' ')", r, exp3, 3);

	/* colon separator */
	r = ft_split("a:b:c", ':');
	check_split(ctx, "split(\"a:b:c\",':')", r, exp_abc, 3);

	/* leading separator */
	r = ft_split(" one two", ' ');
	check_split(ctx, "split(\" one two\",' ')", r, exp2, 2);

	/* trailing separator */
	r = ft_split("one two ", ' ');
	check_split(ctx, "split(\"one two \",' ')", r, exp2, 2);

	/* multiple consecutive separators */
	r = ft_split("one   two", ' ');
	check_split(ctx, "split(\"one   two\",' ')", r, exp2, 2);

	/* all separators */
	r = ft_split("   ", ' ');
	if (r && r[0] == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " split(\"   \",' ') → empty array\n");
		free(r);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " split(\"   \",' ') → expected empty array\n");
		free_split(r);
	}

	/* single char equal to separator → empty array */
	r = ft_split(" ", ' ');
	if (r && r[0] == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " split(\" \",' ') → empty array\n");
		free(r);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " split(\" \",' ') → expected empty array\n");
		free_split(r);
	}

	/* newline separator */
	{
		const char	*exp_nl[] = {"line1", "line2"};
		r = ft_split("line1\nline2", '\n');
		check_split(ctx, "split(\"line1\\nline2\",'\\n')", r, exp_nl, 2);
	}

	/* tab separator */
	{
		const char	*exp_tab[] = {"a", "b", "c"};
		r = ft_split("a\tb\tc", '\t');
		check_split(ctx, "split(\"a\\tb\\tc\",'\\t')", r, exp_tab, 3);
	}

	/* single word, separator appears nowhere */
	{
		const char	*exp_one[] = {"hello"};
		r = ft_split("hello", ':');
		check_split(ctx, "split(\"hello\",':') no sep", r, exp_one, 1);
	}
}
