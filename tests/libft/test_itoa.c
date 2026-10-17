#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

static void	t_itoa(t_ctx *ctx, const char *label, int n, const char *expected)
{
	char	*r;

	r = ft_itoa(n);
	check_str(ctx, label, expected, r);
	free(r);
}

void	test_ft_itoa(t_ctx *ctx)
{
	t_itoa(ctx, "itoa(0)", 0, "0");
	t_itoa(ctx, "itoa(9)", 9, "9");
	t_itoa(ctx, "itoa(-9)", -9, "-9");
	/* digit-count transitions */
	t_itoa(ctx, "itoa(10)", 10, "10");
	t_itoa(ctx, "itoa(-10)", -10, "-10");
	t_itoa(ctx, "itoa(1000)", 1000, "1000");
	t_itoa(ctx, "itoa(2147483647)", 2147483647, "2147483647");
	/* INT_MIN — tricky: abs overflows if done naively */
	t_itoa(ctx, "itoa(-2147483648)", -2147483647 - 1, "-2147483648");
}
