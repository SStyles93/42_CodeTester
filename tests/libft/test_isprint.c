#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_isprint(t_ctx *ctx)
{
	check_bool(ctx, "' ' (32)", isprint(' '), ft_isprint(' '));
	check_bool(ctx, "'~' (126)", isprint('~'), ft_isprint('~'));
	check_bool(ctx, "'A'", isprint('A'), ft_isprint('A'));
	check_bool(ctx, "31", isprint(31), ft_isprint(31));
	check_bool(ctx, "127 (DEL)", isprint(127), ft_isprint(127));
	check_bool(ctx, "0 (NUL)", isprint(0), ft_isprint(0));
	check_bool(ctx, "'\\t' (9)", isprint('\t'), ft_isprint('\t'));
	check_bool(ctx, "200 (>127)", isprint(200), ft_isprint(200));
}
