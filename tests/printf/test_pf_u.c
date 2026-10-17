#include "pf_tester.h"

void	test_pf_u(t_ctx *ctx)
{
	unsigned int	n;
	char			label[48];

	PF(ctx, "%u", 0u);
	PF(ctx, "%u", 1u);
	PF(ctx, "%u", 9u);
	PF(ctx, "%u", 10u);
	PF(ctx, "%u", 42u);
	PF(ctx, "%u", 12345u);
	PF(ctx, "%u", (unsigned int)INT_MAX);
	PF(ctx, "%u", (unsigned int)INT_MAX + 1u);
	PF(ctx, "%u", UINT_MAX);
	PF(ctx, "%u", UINT_MAX - 1);
	PF(ctx, "%u", 4000000000u);

	/* negative ints reinterpreted as unsigned */
	PF(ctx, "%u", -1);
	PF(ctx, "%u", -42);
	PF(ctx, "%u", INT_MIN);
	PF(ctx, "%u", -2147483647);

	PF(ctx, "[%u]", 7u);
	PF(ctx, "%u%u", 1u, 2u);
	PF(ctx, "%u %u %u", 0u, UINT_MAX, 100u);
	PF(ctx, "%u %d", UINT_MAX, -1);

	n = 1;
	while (1)
	{
		snprintf(label, sizeof(label), "\"%%u %%u\", %u, %u", n, n - 1);
		PF_L(ctx, label, "%u %u", n, n - 1);
		if (n > UINT_MAX / 10)
			break ;
		n *= 10;
	}
}
