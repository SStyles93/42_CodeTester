#include "../../include/tester.h"
#include <ctype.h>

int		ft_tolower(int c);

void	test_ft_tolower(t_ctx *ctx)
{
	check_char(ctx, "'A'", (char)tolower('A'), ft_tolower('A'));
	check_char(ctx, "'Z'", (char)tolower('Z'), ft_tolower('Z'));
	check_char(ctx, "'M'", (char)tolower('M'), ft_tolower('M'));
	check_char(ctx, "'a' (no change)", (char)tolower('a'), ft_tolower('a'));
	check_char(ctx, "'z' (no change)", (char)tolower('z'), ft_tolower('z'));
	check_char(ctx, "'1' (no change)", (char)tolower('1'), ft_tolower('1'));
	check_char(ctx, "' ' (no change)", (char)tolower(' '), ft_tolower(' '));
	check_char(ctx, "'{' (no change)", (char)tolower('{'), ft_tolower('{'));
	check_char(ctx, "'@' (no change)", (char)tolower('@'), ft_tolower('@'));
	/* digits and symbols must not change */
	check_char(ctx, "'0' (no change)", (char)tolower('0'), ft_tolower('0'));
	check_char(ctx, "'9' (no change)", (char)tolower('9'), ft_tolower('9'));
	check_char(ctx, "'#' (no change)", (char)tolower('#'), ft_tolower('#'));
	check_char(ctx, "'!' (no change)", (char)tolower('!'), ft_tolower('!'));
	/* all upper-case letters are converted */
	check_char(ctx, "'B' → 'b'", (char)tolower('B'), ft_tolower('B'));
	check_char(ctx, "'N' → 'n'", (char)tolower('N'), ft_tolower('N'));
	check_char(ctx, "'Y' → 'y'", (char)tolower('Y'), ft_tolower('Y'));
}
