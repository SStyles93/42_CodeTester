#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_strncmp(t_ctx *ctx, const char *label,
	const char *s1, const char *s2, size_t n)
{
	int	ref;
	int	got;
	int	rsign;
	int	gsign;

	ref = strncmp(s1, s2, n);
	got = ft_strncmp((char *)s1, (char *)s2, n);
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
			printf("    " RED "✗" RESET " %s: expected sign %d, got sign %d\n",
				label, rsign, gsign);
	}
}

void	test_ft_strncmp(t_ctx *ctx)
{
	t_strncmp(ctx, "equal", "hello", "hello", 5);
	t_strncmp(ctx, "s1 < s2", "abc", "abd", 3);
	t_strncmp(ctx, "s1 > s2", "abd", "abc", 3);
	t_strncmp(ctx, "n=0 → 0", "abc", "xyz", 0);
	t_strncmp(ctx, "equal prefix, differ after n", "hello!", "hello?", 5);
	t_strncmp(ctx, "differ at byte n", "hello!", "hello?", 6);
	t_strncmp(ctx, "s1 shorter (NUL < 'h')", "hi", "high", 4);
	t_strncmp(ctx, "s2 shorter ('h' > NUL)", "high", "hi", 4);
	t_strncmp(ctx, "both empty", "", "", 0);
	t_strncmp(ctx, "empty vs non-empty", "", "a", 1);
	/* n=1 comparisons */
	t_strncmp(ctx, "n=1 equal",   "abc", "abc", 1);
	t_strncmp(ctx, "n=1 less",    "a",   "b",   1);
	t_strncmp(ctx, "n=1 greater", "b",   "a",   1);
	/* n > strlen: equal strings compared past their end */
	t_strncmp(ctx, "n > strlen equal strings", "hi", "hi", 100);
	/* unsigned char comparison: 0xFF (255 as unsigned) > 0x01 (1) */
	{
		char	u1[] = {(char)0xFF, '\0'};
		char	u2[] = {0x01, '\0'};
		t_strncmp(ctx, "0xFF > 0x01 (unsigned)", u1, u2, 1);
	}
	/* differ only at position n: should return 0 */
	t_strncmp(ctx, "equal first n, differ at n+1", "abcX", "abcY", 3);
}
