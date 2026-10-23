#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_strrchr(t_ctx *ctx, const char *label, const char *s, int c)
{
	const char	*ref;
	const char	*got;

	ref = strrchr(s, c);
	got = ft_strrchr(s, c);
	check_offset(ctx, label, s, ref, got);
}

void	test_ft_strrchr(t_ctx *ctx)
{
	t_strrchr(ctx, "last 'l' in \"Hello, World!\"", "Hello, World!", 'l');
	t_strrchr(ctx, "only occurrence at pos 0", "Hello", 'H');
	t_strrchr(ctx, "only at last pos", "xxxb", 'b');
	t_strrchr(ctx, "not found", "Hello", 'x');
	t_strrchr(ctx, "find NUL", "Hello", '\0');
	t_strrchr(ctx, "'a' in empty string", "", 'a');
}
