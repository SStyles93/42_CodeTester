#include "pf_tester.h"

void	test_pf_percent(t_ctx *ctx)
{
	PF(ctx, "%%");
	PF(ctx, "%%%%");
	PF(ctx, "%%d");
	PF(ctx, "%%%d", 42);
}
