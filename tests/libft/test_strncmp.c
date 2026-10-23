#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_strncmp(t_ctx *ctx, const char *label,
	const char *s1, const char *s2, size_t n)
{
	int	ref;
	int	got;

	ref = strncmp(s1, s2, n);
	got = ft_strncmp((char *)s1, (char *)s2, n);
	check_sign(ctx, label, ref, got);
}

void	test_ft_strncmp(t_ctx *ctx)
{
	t_strncmp(ctx, "equal", "hello", "hello", 5);
	t_strncmp(ctx, "s1 < s2", "abc", "abd", 3);
	t_strncmp(ctx, "s1 > s2", "abd", "abc", 3);
	t_strncmp(ctx, "n=0", "abc", "xyz", 0);
	t_strncmp(ctx, "equal prefix, differ after n", "hello!", "hello?", 5);
	t_strncmp(ctx, "s1 shorter (NUL < 'g')", "hi", "high", 4);
	t_strncmp(ctx, "s2 shorter ('g' > NUL)", "high", "hi", 4);
	/* n > strlen: must stop at NUL, not read past it */
	t_strncmp(ctx, "n > strlen equal strings", "hi", "hi", 100);
	/* unsigned char comparison: 0xFF (255 as unsigned) > 0x01 (1) */
	{
		char	u1[] = {(char)0xFF, '\0'};
		char	u2[] = {0x01, '\0'};
		t_strncmp(ctx, "0xFF > 0x01 (unsigned)", u1, u2, 1);
	}
}
