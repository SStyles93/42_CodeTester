#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_isascii(t_ctx *ctx)
{
	check_bool(ctx, "0", isascii(0), ft_isascii(0));
	check_bool(ctx, "1", isascii(1), ft_isascii(1));
	check_bool(ctx, "64", isascii(64), ft_isascii(64));
	check_bool(ctx, "127", isascii(127), ft_isascii(127));
	check_bool(ctx, "128", isascii(128), ft_isascii(128));
	check_bool(ctx, "255", isascii(255), ft_isascii(255));
	check_bool(ctx, "-1", isascii(-1), ft_isascii(-1));
	check_bool(ctx, "'A'", isascii('A'), ft_isascii('A'));
	/* additional boundary and extreme values */
	check_bool(ctx, "126 ('~')", isascii(126), ft_isascii(126));
	check_bool(ctx, "200 (>127)",  isascii(200), ft_isascii(200));
	check_bool(ctx, "-2 (negative)", isascii(-2), ft_isascii(-2));
	check_bool(ctx, "32 (space)",  isascii(32),  ft_isascii(32));
	check_bool(ctx, "33 ('!')",    isascii(33),  ft_isascii(33));
}
