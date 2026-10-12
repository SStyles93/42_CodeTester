#include "../../include/tester.h"
#include <ctype.h>

int	ft_isdigit(int c);

void	test_ft_isdigit(t_ctx *ctx)
{
	check_bool(ctx, "'0'", isdigit('0'), ft_isdigit('0'));
	check_bool(ctx, "'5'", isdigit('5'), ft_isdigit('5'));
	check_bool(ctx, "'9'", isdigit('9'), ft_isdigit('9'));
	check_bool(ctx, "'a'", isdigit('a'), ft_isdigit('a'));
	check_bool(ctx, "'Z'", isdigit('Z'), ft_isdigit('Z'));
	check_bool(ctx, "' '", isdigit(' '), ft_isdigit(' '));
	check_bool(ctx, "0 (NUL)", isdigit(0), ft_isdigit(0));
	check_bool(ctx, "'/'", isdigit('/'), ft_isdigit('/'));
	check_bool(ctx, "':'", isdigit(':'), ft_isdigit(':'));
	/* all digit chars */
	check_bool(ctx, "'1'", isdigit('1'), ft_isdigit('1'));
	check_bool(ctx, "'4'", isdigit('4'), ft_isdigit('4'));
	check_bool(ctx, "'8'", isdigit('8'), ft_isdigit('8'));
	/* high bytes and negative */
	check_bool(ctx, "200 (>127)", isdigit(200), ft_isdigit(200));
	check_bool(ctx, "-1 (EOF-like)", isdigit(-1), ft_isdigit(-1));
	/* letters that look like digits on-screen */
	check_bool(ctx, "'l' (lowercase L)", isdigit('l'), ft_isdigit('l'));
	check_bool(ctx, "'O' (uppercase O)", isdigit('O'), ft_isdigit('O'));
}
