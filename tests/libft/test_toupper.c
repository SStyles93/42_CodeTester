#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_toupper(t_ctx *ctx)
{
	check_char(ctx, "'a'", (char)toupper('a'), ft_toupper('a'));
	check_char(ctx, "'z'", (char)toupper('z'), ft_toupper('z'));
	check_char(ctx, "'m'", (char)toupper('m'), ft_toupper('m'));
	check_char(ctx, "'A' (no change)", (char)toupper('A'), ft_toupper('A'));
	check_char(ctx, "'Z' (no change)", (char)toupper('Z'), ft_toupper('Z'));
	check_char(ctx, "'1' (no change)", (char)toupper('1'), ft_toupper('1'));
	check_char(ctx, "' ' (no change)", (char)toupper(' '), ft_toupper(' '));
	check_char(ctx, "'[' (no change)", (char)toupper('['), ft_toupper('['));
	check_char(ctx, "'`' (no change)", (char)toupper('`'), ft_toupper('`'));
	/* digits and symbols must not change */
	check_char(ctx, "'0' (no change)", (char)toupper('0'), ft_toupper('0'));
	check_char(ctx, "'9' (no change)", (char)toupper('9'), ft_toupper('9'));
	check_char(ctx, "'#' (no change)", (char)toupper('#'), ft_toupper('#'));
	check_char(ctx, "'!' (no change)", (char)toupper('!'), ft_toupper('!'));
	/* all lower-case letters are converted */
	check_char(ctx, "'b' → 'B'", (char)toupper('b'), ft_toupper('b'));
	check_char(ctx, "'n' → 'N'", (char)toupper('n'), ft_toupper('n'));
	check_char(ctx, "'y' → 'Y'", (char)toupper('y'), ft_toupper('y'));
}
