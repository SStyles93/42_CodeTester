#include "pf_tester.h"

static void	spec_values(t_ctx *ctx, const char *fmt)
{
	PF_L(ctx, fmt, fmt, 0);
	PF_L(ctx, fmt, fmt, 9);
	PF_L(ctx, fmt, fmt, 10);
	PF_L(ctx, fmt, fmt, -1);
	PF_L(ctx, fmt, fmt, -10);
	PF_L(ctx, fmt, fmt, INT_MAX);
	PF_L(ctx, fmt, fmt, INT_MIN);
}

void	test_pf_di(t_ctx *ctx)
{
	spec_values(ctx, "%d");
	spec_values(ctx, "%i");
}
