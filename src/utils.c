#include "tester.h"

void	check_bool(t_ctx *ctx, const char *label, int expected, int got)
{
	if (!expected == !got)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s → %s\n", label, got ? "true" : "false");
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected %s, got %s\n",
				label, expected ? "true" : "false", got ? "true" : "false");
	}
}

void	check_int(t_ctx *ctx, const char *label, int expected, int got)
{
	if (expected == got)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s → %d\n", label, got);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected %d, got %d\n",
				label, expected, got);
	}
}

void	check_uint(t_ctx *ctx, const char *label, unsigned int expected, unsigned int got)
{
	if (expected == got)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s → %u\n", label, got);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected %u, got %u\n",
				label, expected, got);
	}
}

void	check_size(t_ctx *ctx, const char *label, size_t expected, size_t got)
{
	if (expected == got)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s → %zu\n", label, got);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected %zu, got %zu\n",
				label, expected, got);
	}
}

void	check_char(t_ctx *ctx, const char *label, char expected, char got)
{
	if (expected == got)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s → '%c'\n", label, got);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected '%c' (%d), got '%c' (%d)\n",
				label, expected, (int)(unsigned char)expected,
				got, (int)(unsigned char)got);
	}
}

void	check_str(t_ctx *ctx, const char *label, const char *expected, const char *got)
{
	int	eq;

	if (expected == NULL && got == NULL)
		eq = 1;
	else if (expected == NULL || got == NULL)
		eq = 0;
	else
		eq = (strcmp(expected, got) == 0);

	if (eq)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s → \"%s\"\n",
				label, got ? got : "(null)");
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected \"%s\", got \"%s\"\n",
				label,
				expected ? expected : "(null)",
				got ? got : "(null)");
	}
}

void	check_mem(t_ctx *ctx, const char *label, const void *expected,
	const void *got, size_t n)
{
	size_t				i;
	const unsigned char	*e;
	const unsigned char	*g;

	if (memcmp(expected, got, n) == 0)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s (%zu bytes)\n", label, n);
		return ;
	}
	ctx->fail++;
	if (ctx->verbose)
	{
		e = (const unsigned char *)expected;
		g = (const unsigned char *)got;
		i = 0;
		while (i < n && e[i] == g[i])
			i++;
		printf("    " RED "✗" RESET " %s: mismatch at byte %zu"
			" (expected 0x%02x, got 0x%02x)\n", label, i, e[i], g[i]);
	}
}

void	check_ptr(t_ctx *ctx, const char *label, const void *expected, const void *got)
{
	if (expected == got)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s\n", label);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected %p, got %p\n",
				label, expected, got);
	}
}

void	check_not_null(t_ctx *ctx, const char *label, const void *got)
{
	if (got != NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s (non-NULL)\n", label);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s → expected non-NULL, got NULL\n", label);
	}
}
