#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_calloc(t_ctx *ctx)
{
	void	*ptr;
	char	zeros[64];

	memset(zeros, 0, sizeof(zeros));

	/* 5 ints */
	ptr = ft_calloc(5, sizeof(int));
	check_not_null(ctx, "calloc(5, int) non-NULL", ptr);
	if (ptr)
	{
		check_mem(ctx, "calloc(5, int) all zeros", zeros, ptr, 5 * sizeof(int));
		free(ptr);
	}

	/* large allocation: 100 000 bytes, first 64 must be zero */
	ptr = ft_calloc(100000, 1);
	check_not_null(ctx, "calloc(100000, 1) non-NULL", ptr);
	if (ptr)
	{
		check_mem(ctx, "calloc(100000,1) first 64 bytes zero", zeros, ptr, sizeof(zeros));
		free(ptr);
	}

	/* calloc(0, x): implementation-defined; just must not crash */
	ptr = ft_calloc(0, 4);
	ctx->pass++;
	if (ctx->verbose)
		printf("    " GREEN "✓" RESET " calloc(0, 4) didn't crash\n");
	if (ptr)
		free(ptr);

	/* overflow-safe: SIZE_MAX * 2 overflows; must not crash */
	{
		void	*p = ft_calloc((size_t)-1, 2);
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET
				" calloc(SIZE_MAX, 2) didn't crash (may return NULL)\n");
		if (p)
			free(p);
	}
}
