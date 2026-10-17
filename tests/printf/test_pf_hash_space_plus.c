#include "pf_tester.h"

/* Bonus part 2: '#', ' ' and '+'. */

static void	hash(t_ctx *ctx)
{
	/* no prefix for 0 */
	PF(ctx, "%#x", 0u);
	PF(ctx, "%#x", 0xbeefu);
	PF(ctx, "%#X", 0xbeefu);
	PF(ctx, "%#10x", 0xffu);
	PF(ctx, "%#-10x|", 0xffu);
	/* zeros go between the prefix and the digits */
	PF(ctx, "%#010x", 0xffu);
	PF(ctx, "%#.5x", 1u);
	PF(ctx, "%#.0x", 0u);
	PF(ctx, "%#08.3x", 0xau);
}

static void	space(t_ctx *ctx)
{
	PF(ctx, "% d", 42);
	PF(ctx, "% d", -42);
	PF(ctx, "% d", 0);
	PF(ctx, "% 5d", 42);
	PF(ctx, "% -5d|", 42);
	PF(ctx, "% 05d", 42);
	PF(ctx, "% .3d", 7);
	PF(ctx, "% .0d", 0);
}

static void	plus(t_ctx *ctx)
{
	PF(ctx, "%+d", 42);
	PF(ctx, "%+d", -42);
	PF(ctx, "%+d", 0);
	PF(ctx, "%+5d", 42);
	PF(ctx, "%+-5d|", 42);
	PF(ctx, "%+05d", 42);
	PF(ctx, "%+.3d", 7);
	PF(ctx, "%+.0d", 0);
	/* '+' wins over ' ', whatever the order */
	PF(ctx, "%+ d", 42);
	PF(ctx, "% +d", 42);
}

void	test_pf_hash_space_plus(t_ctx *ctx)
{
	hash(ctx);
	space(ctx);
	plus(ctx);
}
