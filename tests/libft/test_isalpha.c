#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_isalpha(t_ctx *ctx)
{
	check_bool(ctx, "'a'", isalpha('a'), ft_isalpha('a'));
	check_bool(ctx, "'z'", isalpha('z'), ft_isalpha('z'));
	check_bool(ctx, "'A'", isalpha('A'), ft_isalpha('A'));
	check_bool(ctx, "'Z'", isalpha('Z'), ft_isalpha('Z'));
	check_bool(ctx, "'5'", isalpha('5'), ft_isalpha('5'));
	check_bool(ctx, "0 (NUL)", isalpha(0), ft_isalpha(0));
	/* boundary: chars just outside alpha ranges */
	check_bool(ctx, "'@' (64, before A)", isalpha('@'), ft_isalpha('@'));
	check_bool(ctx, "'[' (91, after Z)",  isalpha('['), ft_isalpha('['));
	check_bool(ctx, "'`' (96, before a)", isalpha('`'), ft_isalpha('`'));
	check_bool(ctx, "'{' (123, after z)", isalpha('{'), ft_isalpha('{'));
	/* high bytes and negative (passed as int) */
	check_bool(ctx, "200 (>127)", isalpha(200), ft_isalpha(200));
	check_bool(ctx, "-1 (EOF-like)", isalpha(-1), ft_isalpha(-1));
}
