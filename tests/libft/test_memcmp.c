#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_memcmp(t_ctx *ctx, const char *label,
	const void *s1, const void *s2, size_t n)
{
	int	ref;
	int	got;

	ref = memcmp(s1, s2, n);
	got = ft_memcmp(s1, s2, n);
	check_sign(ctx, label, ref, got);
}

void	test_ft_memcmp(t_ctx *ctx)
{
	unsigned char	b1[] = {0x00, 0x01, 0xFF};
	unsigned char	b2[] = {0x00, 0x01, 0x00};

	t_memcmp(ctx, "equal", "hello", "hello", 5);
	t_memcmp(ctx, "s1 < s2", "abc", "abd", 3);
	t_memcmp(ctx, "s1 > s2", "abd", "abc", 3);
	t_memcmp(ctx, "n=0", "abc", "xyz", 0);
	/* equal first n bytes, differ after */
	t_memcmp(ctx, "equal n bytes, differ after n", "abcXXX", "abcYYY", 3);
	/* must not stop at 0x00, and must compare as unsigned (0xFF > 0x00) */
	t_memcmp(ctx, "binary: past 0x00, 0xFF > 0x00", b1, b2, 3);
}
