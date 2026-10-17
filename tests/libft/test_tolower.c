#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_tolower(t_ctx *ctx)
{
	check_char(ctx, "'A'", (char)tolower('A'), ft_tolower('A'));
	check_char(ctx, "'Z'", (char)tolower('Z'), ft_tolower('Z'));
	check_char(ctx, "'M'", (char)tolower('M'), ft_tolower('M'));
	check_char(ctx, "'a' (no change)", (char)tolower('a'), ft_tolower('a'));
	check_char(ctx, "'1' (no change)", (char)tolower('1'), ft_tolower('1'));
	/* boundary: chars just outside A-Z */
	check_char(ctx, "'@' (no change)", (char)tolower('@'), ft_tolower('@'));
	check_char(ctx, "'[' (no change)", (char)tolower('['), ft_tolower('['));
}
