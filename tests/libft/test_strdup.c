#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_strdup(t_ctx *ctx)
{
	char	*r;
	const char *src;

	/* empty string */
	r = ft_strdup("");
	check_str(ctx, "ft_strdup(\"\") content", "", r);
	free(r);

	/* content copied and result must be a different pointer */
	src = "hello";
	r = ft_strdup(src);
	check_str(ctx, "ft_strdup(\"hello\") content", "hello", r);
	if (r)
	{
		if ((void *)r != (void *)src)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" ft_strdup: result != src (new allocation)\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" ft_strdup: result == src (not a copy!)\n");
		}
		free(r);
	}

	/* NULL input → NULL return without crash */
	r = ft_strdup(NULL);
	if (r == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " ft_strdup(NULL) → NULL\n");
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " ft_strdup(NULL) → expected NULL\n");
		free(r);
	}
}
