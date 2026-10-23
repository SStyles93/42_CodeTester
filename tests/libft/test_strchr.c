#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

static void	t_strchr(t_ctx *ctx, const char *label, const char *s, int c)
{
	const char	*ref;
	const char	*got;

	ref = strchr(s, c);
	got = ft_strchr(s, c);
	check_offset(ctx, label, s, ref, got);
}

void	test_ft_strchr(t_ctx *ctx)
{
	t_strchr(ctx, "first 'l' in \"Hello\"", "Hello", 'l');
	t_strchr(ctx, "first 'H' at offset 0", "Hello", 'H');
	t_strchr(ctx, "char at end of string", "abcdef", 'f');
	t_strchr(ctx, "not found", "Hello", 'x');
	t_strchr(ctx, "find NUL (end pointer)", "Hello", '\0');
	t_strchr(ctx, "empty string, find NUL", "", '\0');
	/* c > 255: masked to unsigned char — (uchar)('l' + 256) == 'l' */
	t_strchr(ctx, "c=l+256 masked to 'l'", "Hello", 'l' + 256);
}
