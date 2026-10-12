#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>

void	*ft_calloc(size_t nmemb, size_t size);

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

	/* 10 chars */
	ptr = ft_calloc(10, sizeof(char));
	check_not_null(ctx, "calloc(10, char) non-NULL", ptr);
	if (ptr)
	{
		check_mem(ctx, "calloc(10, char) all zeros", zeros, ptr, 10);
		free(ptr);
	}

	/* 1 byte */
	ptr = ft_calloc(1, 1);
	check_not_null(ctx, "calloc(1, 1) non-NULL", ptr);
	if (ptr)
	{
		check_mem(ctx, "calloc(1, 1) zero", zeros, ptr, 1);
		free(ptr);
	}

	/* calloc(0, x): implementation-defined; just must not crash */
	ptr = ft_calloc(0, 4);
	ctx->pass++;
	if (ctx->verbose)
		printf("    " GREEN "✓" RESET " calloc(0, 4) didn't crash\n");
	if (ptr)
		free(ptr);

	/* calloc(0, 0): must not crash */
	ptr = ft_calloc(0, 0);
	ctx->pass++;
	if (ctx->verbose)
		printf("    " GREEN "✓" RESET " calloc(0, 0) didn't crash\n");
	if (ptr)
		free(ptr);

	/* verify returned memory is writable */
	ptr = ft_calloc(64, 1);
	check_not_null(ctx, "calloc(64, 1) writable non-NULL", ptr);
	if (ptr)
	{
		memset(ptr, 0xAB, 64);
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " calloc(64,1): memory is writable\n");
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

	/* stress: many small allocations must all be zero and freeable */
	{
		int		i;
		int		all_ok;

		all_ok = 1;
		i = 0;
		while (i < 64)
		{
			char	*p = ft_calloc(16, 1);
			if (!p) { all_ok = 0; break ; }
			if (memcmp(p, zeros, 16) != 0) all_ok = 0;
			free(p);
			i++;
		}
		if (all_ok)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" calloc: 64×16-byte allocs, all zero and freed\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" calloc: stress alloc failed or not zero\n");
		}
	}
}
