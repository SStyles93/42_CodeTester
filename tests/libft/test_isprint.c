#include "../../include/tester.h"
#include <ctype.h>

int	ft_isprint(int c);

void	test_ft_isprint(t_ctx *ctx)
{
	check_bool(ctx, "' ' (32)", isprint(' '), ft_isprint(' '));
	check_bool(ctx, "'~' (126)", isprint('~'), ft_isprint('~'));
	check_bool(ctx, "'A'", isprint('A'), ft_isprint('A'));
	check_bool(ctx, "'5'", isprint('5'), ft_isprint('5'));
	check_bool(ctx, "31", isprint(31), ft_isprint(31));
	check_bool(ctx, "127 (DEL)", isprint(127), ft_isprint(127));
	check_bool(ctx, "0 (NUL)", isprint(0), ft_isprint(0));
	check_bool(ctx, "'\t' (9)", isprint('\t'), ft_isprint('\t'));
	/* more control characters (not printable) */
	check_bool(ctx, "'\\n' (10)", isprint('\n'), ft_isprint('\n'));
	check_bool(ctx, "'\\r' (13)", isprint('\r'), ft_isprint('\r'));
	check_bool(ctx, "27 (ESC)",   isprint(27),   ft_isprint(27));
	/* more printable chars near boundaries */
	check_bool(ctx, "33 ('!')",   isprint(33),   ft_isprint(33));
	check_bool(ctx, "125 ('}')",  isprint(125),  ft_isprint(125));
	/* high bytes */
	check_bool(ctx, "128 (>127)", isprint(128),  ft_isprint(128));
	check_bool(ctx, "200 (>127)", isprint(200),  ft_isprint(200));
}
