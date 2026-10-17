#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_isalnum(t_ctx *ctx)
{
	check_bool(ctx, "'a'", isalnum('a'), ft_isalnum('a'));
	check_bool(ctx, "'Z'", isalnum('Z'), ft_isalnum('Z'));
	check_bool(ctx, "'5'", isalnum('5'), ft_isalnum('5'));
	check_bool(ctx, "0 (NUL)", isalnum(0), ft_isalnum(0));
	/* boundary: chars just outside alpha/digit ranges */
	check_bool(ctx, "'/' (47, before 0)", isalnum('/'), ft_isalnum('/'));
	check_bool(ctx, "':' (58, after 9)", isalnum(':'), ft_isalnum(':'));
	check_bool(ctx, "'@' (64, before A)", isalnum('@'), ft_isalnum('@'));
	check_bool(ctx, "'[' (91, after Z)", isalnum('['), ft_isalnum('['));
	check_bool(ctx, "'`' (96, before a)", isalnum('`'), ft_isalnum('`'));
	check_bool(ctx, "'{' (123, after z)", isalnum('{'), ft_isalnum('{'));
	/* high bytes and negative */
	check_bool(ctx, "200 (>127)", isalnum(200), ft_isalnum(200));
	check_bool(ctx, "-1 (EOF-like)", isalnum(-1), ft_isalnum(-1));
}
