#include "pf_tester.h"

void	test_pf_s(t_ctx *ctx)
{
	static char	big[100001];
	char		*null_str;

	null_str = NULL;
	PF(ctx, "%s", "hello");
	PF(ctx, "%s", "");
	PF(ctx, "%s", " ");
	PF(ctx, "%s", "a");
	PF(ctx, "%s", "hello world");
	PF(ctx, "%s", "with\nnewline");
	PF(ctx, "%s", "with\ttab");
	PF(ctx, "%s", "%d %s %%");
	PF(ctx, "%s", "\xc3\xa9\xe2\x82\xac\xf0\x9f\x98\x80");
	PF(ctx, "%s", "\x01\x7f\xff");
	PF(ctx, "%s", "embedded\0hidden");
	PF(ctx, "%s %s", "two", "words");
	PF(ctx, "%s%s%s%s", "a", "", "b", "");
	PF(ctx, "-%s-", "");
	PF(ctx, "%s", "The quick brown fox jumps over the lazy dog");

	/* NULL string (glibc prints "(null)") */
	PF(ctx, "%s", null_str);
	PF(ctx, "[%s]", null_str);
	PF(ctx, "%s %s", null_str, "after");
	PF(ctx, "%s%s", null_str, null_str);

	/* very long argument */
	memset(big, 'x', sizeof(big) - 1);
	big[sizeof(big) - 1] = '\0';
	PF_L(ctx, "\"%s\", <100000 x>", "%s", big);
	PF_L(ctx, "\"[%s]\", <100000 x>", "[%s]", big);
}
