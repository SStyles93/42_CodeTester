#include "pf_tester.h"

void	test_pf_percent(t_ctx *ctx)
{
	PF(ctx, "%%");
	PF(ctx, "%%%%");
	PF(ctx, "%%%%%%");
	PF(ctx, "100%%");
	PF(ctx, "%% at start");
	PF(ctx, "at end %%");
	PF(ctx, "%%d");
	PF(ctx, "%%s %%c");
	PF(ctx, "%%%d", 42);
	PF(ctx, "%d%%", 42);
	PF(ctx, "%%%s%%", "mid");
	PF(ctx, "%c%%%c", 'a', 'b');
	PF(ctx, "%% %% %%");
	PF(ctx, "%%\n");
}
