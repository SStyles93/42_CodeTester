#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_isdigit(t_ctx *ctx)
{
	check_bool(ctx, "'0'", isdigit('0'), ft_isdigit('0'));
	check_bool(ctx, "'5'", isdigit('5'), ft_isdigit('5'));
	check_bool(ctx, "'9'", isdigit('9'), ft_isdigit('9'));
	check_bool(ctx, "'a'", isdigit('a'), ft_isdigit('a'));
	check_bool(ctx, "0 (NUL)", isdigit(0), ft_isdigit(0));
	/* boundary: chars just outside the digit range */
	check_bool(ctx, "'/' (47, before 0)", isdigit('/'), ft_isdigit('/'));
	check_bool(ctx, "':' (58, after 9)", isdigit(':'), ft_isdigit(':'));
	/* high bytes and negative */
	check_bool(ctx, "200 (>127)", isdigit(200), ft_isdigit(200));
	check_bool(ctx, "-1 (EOF-like)", isdigit(-1), ft_isdigit(-1));
}
