#include "pf_tester.h"

/* Bonus part 2: '#', ' ' and '+'. */

static void	hash(t_ctx *ctx)
{
	PF(ctx, "%#x", 0u);
	PF(ctx, "%#X", 0u);
	PF(ctx, "%#x", 1u);
	PF(ctx, "%#X", 1u);
	PF(ctx, "%#x", 0xbeefu);
	PF(ctx, "%#X", 0xbeefu);
	PF(ctx, "%#x", UINT_MAX);
	PF(ctx, "%#X", UINT_MAX);
	PF(ctx, "%#10x", 0xffu);
	PF(ctx, "%#-10x|", 0xffu);
	PF(ctx, "%-#10X|", 0xffu);
	PF(ctx, "%#010x", 0xffu);
	PF(ctx, "%0#10X", 0xffu);
	PF(ctx, "%#03x", 0xffu);
	PF(ctx, "%#.5x", 1u);
	PF(ctx, "%#.0x", 0u);
	PF(ctx, "%#.0x", 1u);
	PF(ctx, "%#8.3x", 0xau);
	PF(ctx, "%#-8.3X|", 0xau);
	PF(ctx, "%#08.3x", 0xau);
	PF(ctx, "%#5x", 0u);
	PF(ctx, "%#05x", 0u);
	PF(ctx, "%##x", 0x42u);
	PF(ctx, "%#x %#X %x", 10u, 10u, 10u);
}

static void	space(t_ctx *ctx)
{
	PF(ctx, "% d", 42);
	PF(ctx, "% d", -42);
	PF(ctx, "% d", 0);
	PF(ctx, "% i", INT_MAX);
	PF(ctx, "% i", INT_MIN);
	PF(ctx, "% 5d", 42);
	PF(ctx, "% 5d", -42);
	PF(ctx, "% -5d|", 42);
	PF(ctx, "%- 5d|", -42);
	PF(ctx, "% 05d", 42);
	PF(ctx, "%0 5d", -42);
	PF(ctx, "% .3d", 7);
	PF(ctx, "% .0d", 0);
	PF(ctx, "% 8.3d", 7);
	PF(ctx, "% 1d", 42);
	PF(ctx, "%   d", 42);
	PF(ctx, "% d % d", 1, -1);
}

static void	plus(t_ctx *ctx)
{
	PF(ctx, "%+d", 42);
	PF(ctx, "%+d", -42);
	PF(ctx, "%+d", 0);
	PF(ctx, "%+i", INT_MAX);
	PF(ctx, "%+i", INT_MIN);
	PF(ctx, "%+5d", 42);
	PF(ctx, "%+5d", -42);
	PF(ctx, "%+-5d|", 42);
	PF(ctx, "%-+5d|", 0);
	PF(ctx, "%+05d", 42);
	PF(ctx, "%0+5d", -42);
	PF(ctx, "%+.3d", 7);
	PF(ctx, "%+.0d", 0);
	PF(ctx, "%+8.3d", 7);
	PF(ctx, "%+-8.3d|", -7);
	PF(ctx, "%+1d", 42);
	PF(ctx, "%+++d", 42);
	PF(ctx, "%+12d", INT_MAX);
	PF(ctx, "%+012d", INT_MAX);
	/* '+' wins over ' ' */
	PF(ctx, "%+ d", 42);
	PF(ctx, "% +d", 42);
	PF(ctx, "%+ d", -42);
	PF(ctx, "% +5d", 0);
	PF(ctx, "% +-08.3d|", 5);
	PF(ctx, "%+d %+d", 1, -1);
}

void	test_pf_hash_space_plus(t_ctx *ctx)
{
	hash(ctx);
	space(ctx);
	plus(ctx);
}
