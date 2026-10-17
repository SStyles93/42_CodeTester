#include "pf_tester.h"

void	test_pf_u(t_ctx *ctx)
{
	PF(ctx, "%u", 0u);
	PF(ctx, "%u", 9u);
	PF(ctx, "%u", 10u);
	PF(ctx, "%u", (unsigned int)INT_MAX + 1u);
	PF(ctx, "%u", UINT_MAX);

	/* negative ints reinterpreted as unsigned */
	PF(ctx, "%u", -1);
}
