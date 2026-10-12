#include "../../include/tester.h"
#include <string.h>

/*
** BUGS in ft_striteri.c:
**   1. 'while (s[i++])' — increments i in the condition; the body then uses
**      the already-incremented i. This skips s[0] entirely and passes wrong
**      (off-by-one) indices: f(1,&s[1]), f(2,&s[2]), … instead of
**      f(0,&s[0]), f(1,&s[1]), …
**   2. Check: '!s || !(*s) || !f' — '!(*s)' returns early on any empty
**      string, which is correct, but is redundant since the while would
**      simply not execute on an empty string.
*/

void	ft_striteri(char *s, void (*f)(unsigned int, char *));

/* ---- helper state ---- */
static unsigned int	g_indices[32];
static int			g_call_count;

static void	record(unsigned int i, char *c)
{
	if (g_call_count < 32)
		g_indices[g_call_count] = i;
	g_call_count++;
	(void)c;
}

static void	to_upper_cb(unsigned int i, char *c)
{
	(void)i;
	if (*c >= 'a' && *c <= 'z')
		*c -= 32;
}

void	test_ft_striteri(t_ctx *ctx)
{
	char		buf[64];

	/* in-place to_upper */
	strcpy(buf, "hello");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri to_upper(\"hello\")", "HELLO", buf);

	/* all already upper */
	strcpy(buf, "WORLD");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri to_upper(\"WORLD\") unchanged", "WORLD", buf);

	/* single char */
	strcpy(buf, "a");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri to_upper(\"a\")", "A", buf);

	/* first character is processed (index 0) */
	strcpy(buf, "hello");
	g_call_count = 0;
	ft_striteri(buf, record);
	if (g_call_count > 0 && g_indices[0] == 0)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET
				" striteri: first callback index == 0\n");
	}
	else if (g_call_count == 0)
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET
				" striteri: callback never called on \"hello\"\n");
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET
				" striteri: first callback index should be 0, got %u\n",
				g_indices[0]);
	}

	/* all indices 0-based: 0,1,2,3,4 for "hello" */
	if (g_call_count == 5)
	{
		int	ok;
		int	j;

		ok = 1;
		j = 0;
		while (j < 5)
		{
			if (g_indices[j] != (unsigned int)j)
			{
				ok = 0;
				break ;
			}
			j++;
		}
		if (ok)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET
					" striteri: all indices 0-4 in order\n");
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET
					" striteri: indices out of order\n");
		}
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET
				" striteri: expected 5 calls, got %d\n", g_call_count);
	}

	/* NULL / empty string — must not crash */
	ft_striteri(NULL, to_upper_cb);
	ctx->pass++;
	if (ctx->verbose)
		printf("    " GREEN "✓" RESET " striteri(NULL) didn't crash\n");

	strcpy(buf, "");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri(\"\") unchanged", "", buf);

	/* mixed alpha and non-alpha: only letters should change */
	strcpy(buf, "a1b2c3");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri mixed(\"a1b2c3\")", "A1B2C3", buf);

	/* second callback index must be 1 */
	strcpy(buf, "abc");
	g_call_count = 0;
	ft_striteri(buf, record);
	if (g_call_count >= 2 && g_indices[1] == 1)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET
				" striteri: second callback index == 1\n");
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET
				" striteri: second callback index should be 1, got %u\n",
				g_call_count >= 2 ? g_indices[1] : 999u);
	}

	/* verify single-char string gets exactly one callback at index 0 */
	strcpy(buf, "x");
	g_call_count = 0;
	ft_striteri(buf, record);
	if (g_call_count == 1 && g_indices[0] == 0)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET
				" striteri: single-char: 1 call, index 0\n");
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET
				" striteri: single-char: expected 1 call at idx 0,"
				" got %d calls, first idx %u\n",
				g_call_count, g_call_count ? g_indices[0] : 999u);
	}
}
