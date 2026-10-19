#include "pf_tester.h"

void	test_pf_p(t_ctx *ctx)
{
	int		local;

	/* NULL: Linux/glibc prints "(nil)", macOS prints "0x0" — both accepted */
	PF_L_ALT(ctx, "\"%p\", NULL", "(nil)", "%p", NULL);

	PF(ctx, "%p", (void *)1);
	PF(ctx, "%p", (void *)15);
	PF(ctx, "%p", (void *)16);
	PF(ctx, "%p", (void *)0xabcdef);
	PF(ctx, "%p", (void *)0x100000000);
	PF(ctx, "%p", (void *)LONG_MIN);
	PF(ctx, "%p", (void *)ULONG_MAX);

	/* real address */
	PF(ctx, "%p", (void *)&local);
}
