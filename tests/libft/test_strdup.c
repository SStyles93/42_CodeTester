#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>

/*
** BUGS in ft_strdup.c:
**   1. Function is named 'strdup' instead of 'ft_strdup' — rename it.
**   2. Missing null terminator: sptr[size] = '\0' never written.
**   3. Missing #include <stdlib.h> for malloc.
*/

char	*strdup(const char *s);

void	test_ft_strdup(t_ctx *ctx)
{
	char	*r;
	const char *src;

	/* basic copy */
	r = strdup("hello");
	check_not_null(ctx, "strdup(\"hello\") non-NULL", r);
	check_str(ctx, "strdup(\"hello\") content", "hello", r);
	free(r);

	/* empty string */
	r = strdup("");
	check_not_null(ctx, "strdup(\"\") non-NULL", r);
	check_str(ctx, "strdup(\"\") content", "", r);
	free(r);

	/* single character */
	r = strdup("a");
	check_not_null(ctx, "strdup(\"a\") non-NULL", r);
	check_str(ctx, "strdup(\"a\") content", "a", r);
	free(r);

	/* special chars */
	r = strdup("hello\nworld\t!");
	check_not_null(ctx, "strdup(special) non-NULL", r);
	check_str(ctx, "strdup(special) content", "hello\nworld\t!", r);
	free(r);

	/* long string */
	r = strdup("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
	check_not_null(ctx, "strdup(long) non-NULL", r);
	check_str(ctx, "strdup(long) content",
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit.", r);
	free(r);

	/* result must be a different pointer */
	src = "hello";
	r = strdup(src);
	check_not_null(ctx, "strdup: new allocation", r);
	if (r)
	{
		if ((void *)r != (void *)src)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" strdup: result != src (new allocation)\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" strdup: result == src (not a copy!)\n");
		}
		free(r);
	}

	/* all printable ASCII */
	r = strdup("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	check_not_null(ctx, "strdup(all-printable) non-NULL", r);
	check_str(ctx, "strdup(all-printable) content",
		"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ", r);
	free(r);

	/* verify length of duplicate matches source */
	{
		const char	*src2 = "hello world";
		r = strdup(src2);
		check_not_null(ctx, "strdup length check non-NULL", r);
		if (r)
		{
			check_size(ctx, "strdup: result length matches src",
				strlen(src2), strlen(r));
			free(r);
		}
	}
	/* NUL terminator at expected position */
	r = strdup("abc");
	check_not_null(ctx, "strdup NUL-term non-NULL", r);
	if (r)
	{
		check_char(ctx, "strdup: NUL at pos 3", '\0', r[3]);
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
			char	*p = strdup("stress test");
			if (!p || strcmp(p, "stress test") != 0) ok = 0;
			free(p);
			i++;
		}
		if (ok)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" strdup: 100 stress duplications succeeded\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" strdup: stress duplication failed\n");
		}
	}
}
