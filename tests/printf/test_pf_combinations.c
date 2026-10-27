#include "pf_tester.h"

/*
	Flags, width and precision must not leak into the next conversion/call.
	Each flag on its own is covered by bonus_minus_zero_dot and
	bonus_hash_space_plus.
*/

void	test_pf_combinations(t_ctx *ctx)
{
	PF(ctx, "%-10d|", 1);
	PF(ctx, "%d|", 2);
	PF(ctx, "%08.3d", 3);
	PF(ctx, "%d", 4);
	PF(ctx, "%#x", 5u);
	PF(ctx, "%x", 5u);
	PF(ctx, "%+d", 6);
	PF(ctx, "%d", 6);
	PF(ctx, "%.2s", "eight");
	PF(ctx, "%s", "eight");
	PF(ctx, "%-5d|%d|%05d|%d|%#x|%x|%+d|%d|%.1s|%s", 1, 2, 3, 4, 5u, 6u,
		7, 8, "nine", "ten");
}
