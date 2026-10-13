#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_memcmp(t_ctx *ctx, const char *label,
	const void *s1, const void *s2, size_t n)
{
	int	ref;
	int	got;
	int	rsign;
	int	gsign;

	ref = memcmp(s1, s2, n);
	got = ft_memcmp(s1, s2, n);
	rsign = (ref > 0) - (ref < 0);
	gsign = (got > 0) - (got < 0);
	if (rsign == gsign)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s\n", label);
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s: expected sign %d, got sign %d"
				" (ref=%d, got=%d)\n", label, rsign, gsign, ref, got);
	}
}

void	test_ft_memcmp(t_ctx *ctx)
{
	unsigned char	b1[] = {0x00, 0x01, 0xFF};
	unsigned char	b2[] = {0x00, 0x01, 0x00};

	t_memcmp(ctx, "equal", "hello", "hello", 5);
	t_memcmp(ctx, "s1 < s2", "abc", "abd", 3);
	t_memcmp(ctx, "s1 > s2", "abd", "abc", 3);
	t_memcmp(ctx, "n=0 → 0", "abc", "xyz", 0);
	t_memcmp(ctx, "equal prefix, n within", "hello", "hellp", 4);
	t_memcmp(ctx, "differ at byte n", "hello", "hellp", 5);
	t_memcmp(ctx, "binary: 0xFF > 0x00", b1, b2, 3);
	/* n=1 comparisons */
	t_memcmp(ctx, "n=1 equal", "a", "a", 1);
	t_memcmp(ctx, "n=1 less",  "a", "b", 1);
	t_memcmp(ctx, "n=1 greater", "b", "a", 1);
	/* unsigned comparison: 0xFF (255) > 0x80 (128) */
	{
		unsigned char	u1[] = {0xFF};
		unsigned char	u2[] = {0x80};
		t_memcmp(ctx, "0xFF > 0x80 (unsigned)", u1, u2, 1);
	}
	/* all-zero buffers */
	{
		unsigned char	z1[8] = {0};
		unsigned char	z2[8] = {0};
		t_memcmp(ctx, "all zeros equal", z1, z2, 8);
	}
	/* equal first n bytes, differ after */
	t_memcmp(ctx, "equal n bytes, differ after n", "abcXXX", "abcYYY", 3);
}
