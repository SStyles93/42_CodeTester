#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_strlen(t_ctx *ctx)
{
	check_size(ctx, "\"\"", strlen(""), ft_strlen(""));
	check_size(ctx, "\"a\"", strlen("a"), ft_strlen("a"));
	check_size(ctx, "\"Hello, World!\"", strlen("Hello, World!"), ft_strlen("Hello, World!"));
	/* string containing high bytes (>127) — must stop only at NUL */
	check_size(ctx, "high bytes \\x80\\xff\\x7f",
		strlen("\x80\xff\x7f"), ft_strlen("\x80\xff\x7f"));
	/* stress: 1000-char string */
	{
		char	big[1001];
		memset(big, 'a', 1000);
		big[1000] = '\0';
		check_size(ctx, "1000 'a' chars", 1000, ft_strlen(big));
	}
}
