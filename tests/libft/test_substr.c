#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_substr(t_ctx *ctx)
{
	char	*r;

	/* start=0 */
	r = ft_substr("Hello, World", 0, 5);
	check_str(ctx, "substr(\"Hello, World\",0,5)", "Hello", r);
	free(r);

	/* start in the middle */
	r = ft_substr("Hello, World", 7, 5);
	check_str(ctx, "substr(\"Hello, World\",7,5)", "World", r);
	free(r);

	/* len=0 → empty string */
	r = ft_substr("Hello", 0, 0);
	check_str(ctx, "substr(\"Hello\",0,0)", "", r);
	free(r);

	/* len > remaining → clamp to rest of string */
	r = ft_substr("Hello", 2, 100);
	check_str(ctx, "substr(\"Hello\",2,100)", "llo", r);
	free(r);

	/* start exactly at NUL (index 5 of "hello") → empty string */
	r = ft_substr("hello", 5, 1);
	check_str(ctx, "substr(\"hello\",5,1)", "", r);
	free(r);

	/* start beyond end → empty string */
	r = ft_substr("Hello", 10, 3);
	check_str(ctx, "substr(\"Hello\",10,3)", "", r);
	free(r);

	/* start = UINT_MAX → start + len overflows if done naively */
	r = ft_substr("hello", (unsigned int)-1, 5);
	check_str(ctx, "substr(UINT_MAX,5)", "", r);
	free(r);

	/* NULL string → NULL return without crash */
	r = ft_substr(NULL, 0, 5);
	if (r == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " substr(NULL,0,5) → NULL\n");
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " substr(NULL,0,5) → expected NULL\n");
		free(r);
	}
}
