#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_atoi(t_ctx *ctx)
{
	check_int(ctx, "\"42\"", atoi("42"), ft_atoi("42"));
	check_int(ctx, "\"-42\"", atoi("-42"), ft_atoi("-42"));
	check_int(ctx, "\"  42\"", atoi("  42"), ft_atoi("  42"));
	check_int(ctx, "\"  -42\"", atoi("  -42"), ft_atoi("  -42"));
	check_int(ctx, "\"+42\"", atoi("+42"), ft_atoi("+42"));
	check_int(ctx, "\"0\"", atoi("0"), ft_atoi("0"));
	check_int(ctx, "\"42abc\"", atoi("42abc"), ft_atoi("42abc"));
	check_int(ctx, "\"abc\"", atoi("abc"), ft_atoi("abc"));
	check_int(ctx, "\"\"", atoi(""), ft_atoi(""));
	check_int(ctx, "\"2147483647\"", atoi("2147483647"), ft_atoi("2147483647"));
	check_int(ctx, "\"-2147483648\"", atoi("-2147483648"), ft_atoi("-2147483648"));
	check_int(ctx, "\"\\t\\n42\"", atoi("\t\n42"), ft_atoi("\t\n42"));
	check_int(ctx, "\"1\"", atoi("1"), ft_atoi("1"));
	/* more whitespace types: \f (12), \r (13), \v (11) */
	check_int(ctx, "\"\\f\\r\\v42\"", atoi("\f\r\v42"), ft_atoi("\f\r\v42"));
	/* leading zeros */
	check_int(ctx, "\"007\"",   atoi("007"),  ft_atoi("007"));
	check_int(ctx, "\"0042\"",  atoi("0042"), ft_atoi("0042"));
	/* just a sign character → 0 */
	check_int(ctx, "\"-\"",  atoi("-"),  ft_atoi("-"));
	check_int(ctx, "\"+\"",  atoi("+"),  ft_atoi("+"));
	/* double sign: second sign is not a digit, stops immediately → 0 */
	check_int(ctx, "\"--42\"", atoi("--42"), ft_atoi("--42"));
	check_int(ctx, "\"+-42\"", atoi("+-42"), ft_atoi("+-42"));
	/* all whitespace → 0 */
	check_int(ctx, "\"   \"", atoi("   "), ft_atoi("   "));
	/* whitespace + negative zero */
	check_int(ctx, "\" -0\"", atoi(" -0"), ft_atoi(" -0"));
	/* large whitespace prefix */
	check_int(ctx, "\" \\t\\n  99\"", atoi(" \t\n  99"), ft_atoi(" \t\n  99"));
}
