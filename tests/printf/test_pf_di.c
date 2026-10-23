#include "pf_tester.h"

/* Label: the format and the value as written, e.g. "%d", INT_MIN. */
#define PF_SPEC(ctx, fmt, value) do { \
	char	pf_label_[64]; \
	snprintf(pf_label_, sizeof(pf_label_), "\"%s\", %s", (fmt), #value); \
	PF_L((ctx), pf_label_, (fmt), (value)); \
} while (0)

static void	spec_values(t_ctx *ctx, const char *fmt)
{
	PF_SPEC(ctx, fmt, 0);
	PF_SPEC(ctx, fmt, 9);
	PF_SPEC(ctx, fmt, 10);
	PF_SPEC(ctx, fmt, -1);
	PF_SPEC(ctx, fmt, -10);
	PF_SPEC(ctx, fmt, INT_MAX);
	PF_SPEC(ctx, fmt, INT_MIN);
}

void	test_pf_di(t_ctx *ctx)
{
	spec_values(ctx, "%d");
	spec_values(ctx, "%i");
}
