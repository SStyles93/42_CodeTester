#include "pf_tester.h"

void	test_pf_s(t_ctx *ctx)
{
	static char	big[100001];

	PF(ctx, "%s", "hello");
	PF(ctx, "%s", "");
	/* conversion characters inside the argument are not interpreted */
	PF(ctx, "%s", "%d %s %%");
	PF(ctx, "%s", "\x01\x7f\xff");

	/* very long argument */
	memset(big, 'x', sizeof(big) - 1);
	big[sizeof(big) - 1] = '\0';
	PF_L(ctx, "\"%s\", <100000 x>", "%s", big);
}
