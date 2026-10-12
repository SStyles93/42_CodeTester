#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>

char	*ft_strdup(const char *s);

void	test_ft_strdup(t_ctx *ctx)
{
	char	*r;
	const char *src;

	/* basic copy */
	r = ft_strdup("hello");
	check_not_null(ctx, "ft_strdup(\"hello\") non-NULL", r);
	check_str(ctx, "ft_strdup(\"hello\") content", "hello", r);
	free(r);

	/* empty string */
	r = ft_strdup("");
	check_not_null(ctx, "ft_strdup(\"\") non-NULL", r);
	check_str(ctx, "ft_strdup(\"\") content", "", r);
	free(r);

	/* single character */
	r = ft_strdup("a");
	check_not_null(ctx, "ft_strdup(\"a\") non-NULL", r);
	check_str(ctx, "ft_strdup(\"a\") content", "a", r);
	free(r);

	/* special chars */
	r = ft_strdup("hello\nworld\t!");
	check_not_null(ctx, "ft_strdup(special) non-NULL", r);
	check_str(ctx, "ft_strdup(special) content", "hello\nworld\t!", r);
	free(r);

	/* long string */
	r = ft_strdup("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
	check_not_null(ctx, "ft_strdup(long) non-NULL", r);
	check_str(ctx, "ft_strdup(long) content",
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit.", r);
	free(r);

	/* result must be a different pointer */
	src = "hello";
	r = ft_strdup(src);
	check_not_null(ctx, "ft_strdup: new allocation", r);
	if (r)
	{
		if ((void *)r != (void *)src)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" ft_strdup: result != src (new allocation)\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" ft_strdup: result == src (not a copy!)\n");
		}
		free(r);
	}

	/* all printable ASCII */
	r = ft_strdup("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	check_not_null(ctx, "ft_strdup(all-printable) non-NULL", r);
	check_str(ctx, "ft_strdup(all-printable) content",
		"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ", r);
	free(r);

	/* verify length of duplicate matches source */
	{
		const char	*src2 = "hello world";
		r = ft_strdup(src2);
		check_not_null(ctx, "ft_strdup length check non-NULL", r);
		if (r)
		{
			check_size(ctx, "ft_strdup: result length matches src",
				strlen(src2), strlen(r));
			free(r);
		}
	}
	/* NUL terminator at expected position */
	r = ft_strdup("abc");
	check_not_null(ctx, "ft_strdup NUL-term non-NULL", r);
	if (r)
	{
		check_char(ctx, "ft_strdup: NUL at pos 3", '\0', r[3]);
		free(r);
	}
	/* stress: 100 duplications of same string, all freeable */
	{
		int		i;
		int		ok;

		ok = 1;
		i = 0;
		while (i < 100)
		{
			char	*p = ft_strdup("stress test");
			if (!p || strcmp(p, "stress test") != 0) ok = 0;
			free(p);
			i++;
		}
		if (ok)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" ft_strdup: 100 stress duplications succeeded\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" ft_strdup: stress duplication failed\n");
		}
	}
}
