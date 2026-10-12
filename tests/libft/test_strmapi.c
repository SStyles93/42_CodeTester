#include "../../include/tester.h"
#include <stdlib.h>

/*
** BUGS in ft_strmapi.c:
**   1. Calls ft_strdup() but libft.h only declares strdup() — rename
**      the call to strdup() to match the (buggy) ft_strdup.c symbol.
**      Fix ft_strdup.c to be named ft_strdup and update libft.h.
*/

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

static char	to_upper(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static char	rot13(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return ('a' + (c - 'a' + 13) % 26);
	if (c >= 'A' && c <= 'Z')
		return ('A' + (c - 'A' + 13) % 26);
	return (c);
}

/* Records the last index received by the callback */
static unsigned int	g_strmapi_idx;

static char	record_idx(unsigned int i, char c)
{
	g_strmapi_idx = i;
	return (c);
}

void	test_ft_strmapi(t_ctx *ctx)
{
	char		*r;

	/* to_upper */
	r = ft_strmapi("hello", to_upper);
	check_not_null(ctx, "strmapi to_upper non-NULL", r);
	check_str(ctx, "strmapi(\"hello\", to_upper)", "HELLO", r);
	free(r);

	/* already uppercase — identity */
	r = ft_strmapi("HELLO", to_upper);
	check_not_null(ctx, "strmapi HELLO non-NULL", r);
	check_str(ctx, "strmapi(\"HELLO\", to_upper)", "HELLO", r);
	free(r);

	/* empty string */
	r = ft_strmapi("", to_upper);
	check_not_null(ctx, "strmapi(\"\") non-NULL", r);
	check_str(ctx, "strmapi(\"\", to_upper)", "", r);
	free(r);

	/* rot13 */
	r = ft_strmapi("hello", rot13);
	check_not_null(ctx, "strmapi rot13 non-NULL", r);
	check_str(ctx, "strmapi(\"hello\", rot13)", "uryyb", r);
	free(r);

	/* original string is not modified */
	{
		const char *orig = "hello";
		r = ft_strmapi(orig, to_upper);
		check_not_null(ctx, "strmapi: no-modify orig non-NULL", r);
		check_str(ctx, "strmapi: orig unchanged", "hello", orig);
		free(r);
	}

	/* index passed to callback is 0-based */
	g_strmapi_idx = 999;
	r = ft_strmapi("a", record_idx);
	check_not_null(ctx, "strmapi index-check non-NULL", r);
	if (r)
	{
		if (g_strmapi_idx == 0)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" strmapi: first char index == 0\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" strmapi: first char index should be 0, got %u\n",
					g_strmapi_idx);
		}
		free(r);
	}

	/* single char */
	r = ft_strmapi("z", to_upper);
	check_not_null(ctx, "strmapi single-char non-NULL", r);
	check_str(ctx, "strmapi(\"z\", to_upper)", "Z", r);
	free(r);

	/* last index passed to callback must be strlen-1 */
	g_strmapi_idx = 999;
	r = ft_strmapi("hello", record_idx);
	check_not_null(ctx, "strmapi last-idx non-NULL", r);
	if (r)
	{
		if (g_strmapi_idx == 4)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" strmapi: last char index == 4 (strlen-1)\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" strmapi: last char index should be 4, got %u\n",
					g_strmapi_idx);
		}
		free(r);
	}

	/* string with digits and symbols: only letters should change */
	r = ft_strmapi("abc123!!", to_upper);
	check_not_null(ctx, "strmapi digits+sym non-NULL", r);
	if (r) { check_str(ctx, "strmapi(\"abc123!!\", to_upper)", "ABC123!!", r); free(r); }

	/* result is NUL-terminated */
	r = ft_strmapi("hi", to_upper);
	check_not_null(ctx, "strmapi NUL-term non-NULL", r);
	if (r)
	{
		check_char(ctx, "strmapi: NUL at pos 2", '\0', r[2]);
		free(r);
	}
}
