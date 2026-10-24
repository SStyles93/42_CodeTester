#include "pf_tester.h"

/* Label: the format and the value as written, e.g. "%x", UINT_MAX. */
#define PF_SPEC(ctx, fmt, value) do { \
	char	pf_label_[64]; \
	snprintf(pf_label_, sizeof(pf_label_), "\"%s\", %s", (fmt), #value); \
	PF_L((ctx), pf_label_, (fmt), (value)); \
} while (0)

static void	spec_values(t_ctx *ctx, const char *fmt)
{
	PF_SPEC(ctx, fmt, 0u);
	PF_SPEC(ctx, fmt, 9u);
	PF_SPEC(ctx, fmt, 10u);
	PF_SPEC(ctx, fmt, 15u);
	PF_SPEC(ctx, fmt, 16u);
	PF_SPEC(ctx, fmt, 0xabcdefu);
	PF_SPEC(ctx, fmt, UINT_MAX);
	PF_SPEC(ctx, fmt, INT_MIN);
}

void	test_pf_x(t_ctx *ctx)
{
	spec_values(ctx, "%x");
	spec_values(ctx, "%X");
}
