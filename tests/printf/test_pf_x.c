#include "pf_tester.h"

static void	spec_values(t_ctx *ctx, const char *fmt)
{
	PF_L(ctx, fmt, fmt, 0u);
	PF_L(ctx, fmt, fmt, 1u);
	PF_L(ctx, fmt, fmt, 9u);
	PF_L(ctx, fmt, fmt, 10u);
	PF_L(ctx, fmt, fmt, 15u);
	PF_L(ctx, fmt, fmt, 16u);
	PF_L(ctx, fmt, fmt, 42u);
	PF_L(ctx, fmt, fmt, 255u);
	PF_L(ctx, fmt, fmt, 256u);
	PF_L(ctx, fmt, fmt, 4095u);
	PF_L(ctx, fmt, fmt, 0xabcdefu);
	PF_L(ctx, fmt, fmt, 0xdeadbeefu);
	PF_L(ctx, fmt, fmt, 0x10000000u);
	PF_L(ctx, fmt, fmt, (unsigned int)INT_MAX);
	PF_L(ctx, fmt, fmt, UINT_MAX);
	PF_L(ctx, fmt, fmt, -1);
	PF_L(ctx, fmt, fmt, INT_MIN);
	PF_L(ctx, fmt, fmt, -42);
}

void	test_pf_x(t_ctx *ctx)
{
	unsigned int	n;
	char			label[48];

	spec_values(ctx, "%x");
	spec_values(ctx, "%X");
	spec_values(ctx, "[%x]");
	spec_values(ctx, "<%X>");
	PF(ctx, "%x %X", 0xabcdefu, 0xabcdefu);
	PF(ctx, "%x%x%x", 1u, 2u, 3u);
	PF(ctx, "%X%X%X", 10u, 11u, 12u);
	PF(ctx, "%x %u %X", 255u, 255u, 255u);

	n = 1;
	while (1)
	{
		snprintf(label, sizeof(label), "\"%%x %%X\", %u, %u", n, n - 1);
		PF_L(ctx, label, "%x %X", n, n - 1);
		if (n > UINT_MAX / 16)
			break ;
		n *= 16;
	}
}
