#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_strtrim(t_ctx *ctx)
{
	char	*r;

	/* trim both sides with single char */
	r = ft_strtrim("  hello  ", " ");
	check_str(ctx, "strtrim(\"  hello  \",\" \")", "hello", r);
	free(r);

	/* leading trim only */
	r = ft_strtrim("   hello", " ");
	check_str(ctx, "strtrim(\"   hello\",\" \")", "hello", r);
	free(r);

	/* trailing trim only */
	r = ft_strtrim("hello   ", " ");
	check_str(ctx, "strtrim(\"hello   \",\" \")", "hello", r);
	free(r);

	/* multi-char set: internal set chars must be preserved */
	r = ft_strtrim("abXYZba", "ab");
	check_str(ctx, "strtrim(\"abXYZba\",\"ab\")", "XYZ", r);
	free(r);

	/* no trimming needed */
	r = ft_strtrim("hello", "x");
	check_str(ctx, "strtrim(\"hello\",\"x\")", "hello", r);
	free(r);

	/* empty set → no trimming */
	r = ft_strtrim("hello", "");
	check_str(ctx, "strtrim(\"hello\",\"\")", "hello", r);
	free(r);

	/* empty input */
	r = ft_strtrim("", " ");
	check_str(ctx, "strtrim(\"\",\" \")", "", r);
	free(r);

	/* entire string in set */
	r = ft_strtrim("   ", " ");
	check_str(ctx, "strtrim(\"   \",\" \")", "", r);
	free(r);

	/* NULL s1 → NULL return without crash */
	r = ft_strtrim(NULL, " ");
	if (r == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " strtrim(NULL,\" \") → NULL\n");
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " strtrim(NULL,\" \") → expected NULL\n");
		free(r);
	}
}
