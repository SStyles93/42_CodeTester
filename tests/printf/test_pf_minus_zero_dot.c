#include "pf_tester.h"

/* Bonus part 1: '-', '0', '.' and minimum field width. */

static void	width_only(t_ctx *ctx)
{
	PF(ctx, "%5d", 42);
	PF(ctx, "%5d", -42);
	/* width smaller than the output */
	PF(ctx, "%1d", 42);
	PF(ctx, "%5u", 42u);
	PF(ctx, "%8x", 0xbeefu);
	PF(ctx, "%5s", "abc");
	PF(ctx, "%3c", 'a');
	PF(ctx, "%3c", '\0');
	PF(ctx, "%20p", (void *)0xdeadbeef);
	PF(ctx, "%10p", NULL);
	PF(ctx, "%100000c", 'w');
}

static void	minus(t_ctx *ctx)
{
	PF(ctx, "%-5d|", 42);
	PF(ctx, "%-5d|", -42);
	PF(ctx, "%-5u|", 42u);
	PF(ctx, "%-8x|", 0xbeefu);
	PF(ctx, "%-5s|", "abc");
	PF(ctx, "%-3c|", 'a');
	PF(ctx, "%-3c|", '\0');
	PF(ctx, "%-20p|", (void *)0xdeadbeef);
	/* repeated flag, flag without width */
	PF(ctx, "%---5d|", 7);
	PF(ctx, "%-d|", 7);
}

static void	zero(t_ctx *ctx)
{
	PF(ctx, "%05d", 42);
	/* sign goes before the zeros */
	PF(ctx, "%05d", -42);
	PF(ctx, "%05u", 42u);
	PF(ctx, "%08x", 0xbeefu);
	PF(ctx, "%0d", 5);
	/* '-' wins over '0', whatever the order */
	PF(ctx, "%-05d|", 42);
	PF(ctx, "%0-5d|", -42);
	/* a precision disables '0' for integers */
	PF(ctx, "%08.3d", 42);
	PF(ctx, "%08.3d", -42);
	PF(ctx, "%08.0d", 0);
}

static void	precision(t_ctx *ctx)
{
	/* precision 0 with value 0 prints nothing */
	PF(ctx, "%.0d", 0);
	PF(ctx, "%.d", 0);
	PF(ctx, "%.0d", 1);
	PF(ctx, "%.3d", 7);
	PF(ctx, "%.3d", -7);
	PF(ctx, "%.3d", 12345);
	PF(ctx, "%.1000d", -1);
	PF(ctx, "%.0u", 0u);
	PF(ctx, "%.5u", 42u);
	PF(ctx, "%.0x", 0u);
	PF(ctx, "%.4x", 0xfu);
	/* width + precision */
	PF(ctx, "%8.3d", 42);
	PF(ctx, "%8.3d", -42);
	PF(ctx, "%-8.3d|", -42);
	PF(ctx, "%3.8d", 42);
	PF(ctx, "%5.0d", 0);
	PF(ctx, "%10.5x", 0xabcu);
	/* strings */
	PF(ctx, "%.0s", "hello");
	PF(ctx, "%.3s", "hello");
	PF(ctx, "%.10s", "hello");
	PF(ctx, "%10.3s|", "hello");
	PF(ctx, "%-10.3s|", "hello");
}

void	test_pf_minus_zero_dot(t_ctx *ctx)
{
	width_only(ctx);
	minus(ctx);
	zero(ctx);
	precision(ctx);
}
