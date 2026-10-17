#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_strchr(t_ctx *ctx, const char *label, const char *s, int c)
{
	const char	*ref;
	const char	*got;

	ref = strchr(s, c);
	got = ft_strchr(s, c);
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

void	test_ft_strchr(t_ctx *ctx)
{
	t_strchr(ctx, "first 'l' in \"Hello\"", "Hello", 'l');
	t_strchr(ctx, "first 'H' at offset 0", "Hello", 'H');
	t_strchr(ctx, "char at end of string", "abcdef", 'f');
	t_strchr(ctx, "not found", "Hello", 'x');
	t_strchr(ctx, "find NUL (end pointer)", "Hello", '\0');
	t_strchr(ctx, "empty string, find NUL", "", '\0');
	/* c > 255: masked to unsigned char — (uchar)('l' + 256) == 'l' */
	t_strchr(ctx, "c=l+256 masked to 'l'", "Hello", 'l' + 256);
}
