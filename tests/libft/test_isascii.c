#include "../../include/tester.h"
#include <ctype.h>
#include "../../include/weak_functions.h"

void	test_ft_isascii(t_ctx *ctx)
{
	check_bool(ctx, "0", isascii(0), ft_isascii(0));
	check_bool(ctx, "'A'", isascii('A'), ft_isascii('A'));
	check_bool(ctx, "127", isascii(127), ft_isascii(127));
	check_bool(ctx, "128", isascii(128), ft_isascii(128));
	check_bool(ctx, "-1", isascii(-1), ft_isascii(-1));
}
