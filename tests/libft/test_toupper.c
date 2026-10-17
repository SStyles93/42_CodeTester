#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_toupper(t_ctx *ctx)
{
	check_char(ctx, "'a'", (char)toupper('a'), ft_toupper('a'));
	check_char(ctx, "'z'", (char)toupper('z'), ft_toupper('z'));
	check_char(ctx, "'m'", (char)toupper('m'), ft_toupper('m'));
	check_char(ctx, "'A' (no change)", (char)toupper('A'), ft_toupper('A'));
	check_char(ctx, "'1' (no change)", (char)toupper('1'), ft_toupper('1'));
	/* boundary: chars just outside a-z */
	check_char(ctx, "'`' (no change)", (char)toupper('`'), ft_toupper('`'));
	check_char(ctx, "'{' (no change)", (char)toupper('{'), ft_toupper('{'));
}
