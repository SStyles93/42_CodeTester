#include "pf_tester.h"

static void	spec_values(t_ctx *ctx, const char *fmt)
{
	PF_L(ctx, fmt, fmt, 0u);
	PF_L(ctx, fmt, fmt, 9u);
	PF_L(ctx, fmt, fmt, 10u);
	PF_L(ctx, fmt, fmt, 15u);
	PF_L(ctx, fmt, fmt, 16u);
	PF_L(ctx, fmt, fmt, 0xabcdefu);
	PF_L(ctx, fmt, fmt, UINT_MAX);
	PF_L(ctx, fmt, fmt, INT_MIN);
}

void	test_pf_x(t_ctx *ctx)
{
	spec_values(ctx, "%x");
	spec_values(ctx, "%X");
}
