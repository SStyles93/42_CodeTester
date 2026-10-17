#include "pf_tester.h"

void	test_pf_c(t_ctx *ctx)
{
	PF(ctx, "%c", 'a');
	PF(ctx, "%c", '%');

	/* NUL is written and counted */
	PF(ctx, "%c", '\0');
	PF(ctx, "a%cb", '\0');

	/* int argument converted to unsigned char */
	PF(ctx, "%c", 128);
	PF(ctx, "%c", 255);
	PF(ctx, "%c", -1);
	PF(ctx, "%c", 256 + 'A');
}
