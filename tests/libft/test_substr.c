#include "../../include/tester.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len);

void	test_ft_substr(t_ctx *ctx)
{
	char	*r;

	/* start=0, len covers exact word */
	r = ft_substr("Hello, World", 0, 5);
	check_not_null(ctx, "substr(0,5) non-NULL", r);
	check_str(ctx, "substr(\"Hello, World\",0,5)", "Hello", r);
	free(r);

	/* start in the middle */
	r = ft_substr("Hello, World", 7, 5);
	check_not_null(ctx, "substr(7,5) non-NULL", r);
	check_str(ctx, "substr(\"Hello, World\",7,5)", "World", r);
	free(r);

	/* len=0 → empty string */
	r = ft_substr("Hello", 0, 0);
	check_not_null(ctx, "substr(0,0) non-NULL", r);
	check_str(ctx, "substr(\"Hello\",0,0)", "", r);
	free(r);

	/* start beyond end → empty string */
	r = ft_substr("Hello", 10, 3);
	check_not_null(ctx, "substr(10,3) non-NULL", r);
	check_str(ctx, "substr(\"Hello\",10,3)", "", r);
	free(r);

	/* len > remaining → clamp to rest of string */
	r = ft_substr("Hello", 2, 100);
	check_not_null(ctx, "substr(2,100) non-NULL", r);
	check_str(ctx, "substr(\"Hello\",2,100)", "llo", r);
	free(r);

	/* empty input string */
	r = ft_substr("", 0, 5);
	check_not_null(ctx, "substr(\"\",0,5) non-NULL", r);
	check_str(ctx, "substr(\"\",0,5)", "", r);
	free(r);

	/* single char */
	r = ft_substr("abc", 1, 1);
	check_not_null(ctx, "substr(1,1) non-NULL", r);
	check_str(ctx, "substr(\"abc\",1,1)", "b", r);
	free(r);

	/* full string copy (len == strlen) */
	r = ft_substr("hello", 0, 5);
	check_not_null(ctx, "substr(0,5) full non-NULL", r);
	check_str(ctx, "substr(\"hello\",0,5) full", "hello", r);
	free(r);

	/* start=0, len=1 */
	r = ft_substr("world", 0, 1);
	check_not_null(ctx, "substr(0,1) non-NULL", r);
	check_str(ctx, "substr(\"world\",0,1)", "w", r);
	free(r);

	/* start at last char (index 4 of "hello") */
	r = ft_substr("hello", 4, 1);
	check_not_null(ctx, "substr(4,1) non-NULL", r);
	if (r) { check_str(ctx, "substr(\"hello\",4,1)", "o", r); free(r); }

	/* start exactly at NUL (index 5 of "hello") → empty string */
	r = ft_substr("hello", 5, 1);
	check_not_null(ctx, "substr(5,1) non-NULL", r);
	if (r) { check_str(ctx, "substr(\"hello\",5,1)", "", r); free(r); }

	/* start = UINT_MAX → well past end → empty string */
	r = ft_substr("hello", (unsigned int)-1, 5);
	check_not_null(ctx, "substr(UINT_MAX,5) non-NULL", r);
	if (r) { check_str(ctx, "substr(UINT_MAX,5)", "", r); free(r); }

	/* stress: 50 allocations with different start/len, all freeable */
	{
		const char	*s = "abcdefghijklmnopqrstuvwxyz";
		unsigned int	i;
		int		ok;

		ok = 1;
		i = 0;
		while (i < 26)
		{
			char	*p = ft_substr(s, i, 1);
			if (!p) { ok = 0; break ; }
			if (p[0] != s[i] || p[1] != '\0') ok = 0;
			free(p);
			i++;
		}
		if (ok)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" substr: 26 single-char extractions, all correct\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" substr: single-char extraction stress failed\n");
		}
	}
}
