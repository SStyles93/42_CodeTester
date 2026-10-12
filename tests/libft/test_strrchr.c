#include "../../include/tester.h"
#include <string.h>

char	*ft_strrchr(const char *s, int c);

static void	t_strrchr(t_ctx *ctx, const char *label, const char *s, int c)
{
	const char	*ref;
	const char	*got;

	ref = strrchr(s, c);
	got = ft_strrchr(s, c);
	if (ref == NULL && got == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s (both NULL)\n", label);
	}
	else if (ref == NULL || got == NULL)
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s: expected %s, got %s\n",
				label, ref ? "ptr" : "NULL", got ? "ptr" : "NULL");
	}
	else if ((ref - s) == (got - s))
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s (offset %ld)\n",
				label, (long)(ref - s));
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s: expected offset %ld, got %ld\n",
				label, (long)(ref - s), (long)(got - s));
	}
}

void	test_ft_strrchr(t_ctx *ctx)
{
	t_strrchr(ctx, "last 'l' in \"Hello\"", "Hello", 'l');
	t_strrchr(ctx, "last 'l' in \"Hello, World!\"", "Hello, World!", 'l');
	t_strrchr(ctx, "only occurrence 'H'", "Hello", 'H');
	t_strrchr(ctx, "not found", "Hello", 'x');
	t_strrchr(ctx, "find NUL", "Hello", '\0');
	t_strrchr(ctx, "empty string", "", 'a');
	t_strrchr(ctx, "last of repeated char", "abcabc", 'a');
	/* single-char strings */
	t_strrchr(ctx, "single char, found",     "a", 'a');
	t_strrchr(ctx, "single char, not found", "a", 'b');
	/* all same chars: last occurrence is the last char */
	t_strrchr(ctx, "all same, last occurrence", "aaaa", 'a');
	/* char at position 0 only */
	t_strrchr(ctx, "only at pos 0", "axxx", 'a');
	/* char at last position only */
	t_strrchr(ctx, "only at last pos", "xxxb", 'b');
}
