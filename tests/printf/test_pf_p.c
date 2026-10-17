#include "pf_tester.h"

void	test_pf_p(t_ctx *ctx)
{
	int		local;
	char	*heap;

	heap = malloc(16);
	/* NULL ("(nil)" on Linux, "0x0" on macOS) */
	PF(ctx, "%p", NULL);
	PF(ctx, "%p", (void *)0);
	PF(ctx, "[%p]", NULL);
	PF(ctx, "%p %p", NULL, NULL);

	PF(ctx, "%p", (void *)1);
	PF(ctx, "%p", (void *)15);
	PF(ctx, "%p", (void *)16);
	PF(ctx, "%p", (void *)255);
	PF(ctx, "%p", (void *)0xdeadbeef);
	PF(ctx, "%p", (void *)0xabcdef);
	PF(ctx, "%p", (void *)0x7fffffff);
	PF(ctx, "%p", (void *)0x100000000);
	PF(ctx, "%p", (void *)LONG_MAX);
	PF(ctx, "%p", (void *)LONG_MIN);
	PF(ctx, "%p", (void *)ULONG_MAX);
	PF(ctx, "%p", (void *)-1);
	PF(ctx, "%p", (void *)-42);
	PF(ctx, "%p", (void *)INT_MIN);
	PF(ctx, "%p", (void *)(unsigned long)UINT_MAX);

	/* real addresses */
	PF(ctx, "%p", (void *)&local);
	PF(ctx, "%p", (void *)heap);
	PF(ctx, "%p", (void *)"string literal");
	PF(ctx, "%p", (void *)ctx);
	PF(ctx, "%p %p %p", (void *)&local, (void *)heap, NULL);
	PF(ctx, "ptr=%p int=%d", (void *)&local, 42);
	free(heap);
}
