#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_atoi(t_ctx *ctx)
{
	check_int(ctx, "\"42\"", atoi("42"), ft_atoi("42"));
	check_int(ctx, "\"-42\"", atoi("-42"), ft_atoi("-42"));
	check_int(ctx, "\"+42\"", atoi("+42"), ft_atoi("+42"));
	check_int(ctx, "\"0\"", atoi("0"), ft_atoi("0"));
	/* every whitespace type before a sign: ' ' \t \n \v \f \r */
	check_int(ctx, "\" \\t\\n\\v\\f\\r-42\"", atoi(" \t\n\v\f\r-42"),
		ft_atoi(" \t\n\v\f\r-42"));
	check_int(ctx, "\"42abc\"", atoi("42abc"), ft_atoi("42abc"));
	check_int(ctx, "\"abc\"", atoi("abc"), ft_atoi("abc"));
	check_int(ctx, "\"\"", atoi(""), ft_atoi(""));
	check_int(ctx, "\"   \"", atoi("   "), ft_atoi("   "));
	check_int(ctx, "\"2147483647\"", atoi("2147483647"), ft_atoi("2147483647"));
	check_int(ctx, "\"-2147483648\"", atoi("-2147483648"), ft_atoi("-2147483648"));
	check_int(ctx, "\"0042\"", atoi("0042"), ft_atoi("0042"));
	/* just a sign character → 0 */
	check_int(ctx, "\"-\"", atoi("-"), ft_atoi("-"));
	/* double sign: second sign is not a digit, stops immediately → 0 */
	check_int(ctx, "\"+-42\"", atoi("+-42"), ft_atoi("+-42"));
}
