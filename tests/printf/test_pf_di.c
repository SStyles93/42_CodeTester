#include "pf_tester.h"

static void	powers_of_ten(t_ctx *ctx)
{
	int		n;
	char	label[48];

	n = 1;
	while (1)
	{
		snprintf(label, sizeof(label), "\"%%d %%i\", %d, %d", n, -n);
		PF_L(ctx, label, "%d %i", n, -n);
		snprintf(label, sizeof(label), "\"%%d\", %d", n - 1);
		PF_L(ctx, label, "%d", n - 1);
		if (n > INT_MAX / 10)
			break ;
		n *= 10;
	}
}

static void	spec_values(t_ctx *ctx, const char *fmt)
{
	PF_L(ctx, fmt, fmt, 0);
	PF_L(ctx, fmt, fmt, 1);
	PF_L(ctx, fmt, fmt, -1);
	PF_L(ctx, fmt, fmt, 9);
	PF_L(ctx, fmt, fmt, 10);
	PF_L(ctx, fmt, fmt, -10);
	PF_L(ctx, fmt, fmt, 42);
	PF_L(ctx, fmt, fmt, -42);
	PF_L(ctx, fmt, fmt, 12345);
	PF_L(ctx, fmt, fmt, -98765);
	PF_L(ctx, fmt, fmt, INT_MAX);
	PF_L(ctx, fmt, fmt, INT_MIN);
	PF_L(ctx, fmt, fmt, INT_MAX - 1);
	PF_L(ctx, fmt, fmt, INT_MIN + 1);
	PF_L(ctx, fmt, fmt, 'a');
	PF_L(ctx, fmt, fmt, (short)-32768);
	PF_L(ctx, fmt, fmt, (char)-128);
}

void	test_pf_di(t_ctx *ctx)
{
	spec_values(ctx, "%d");
	spec_values(ctx, "%i");
	spec_values(ctx, "[%d]");
	spec_values(ctx, "[%i]");
	PF(ctx, "%d%d", 1, -1);
	PF(ctx, "%i%i%i", INT_MIN, 0, INT_MAX);
	PF(ctx, "%d %i %d %i", -2147483647, 2147483647, 0, -0);
	PF(ctx, "%d", (int)2147483648u);
	PF(ctx, "%d", (int)4294967295u);
	PF(ctx, "%i", (int)3000000000u);
	PF(ctx, "%d is the answer", 42);
	PF(ctx, "min %d max %d", INT_MIN, INT_MAX);
	powers_of_ten(ctx);
}
