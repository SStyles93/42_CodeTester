#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

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

	/* in-place edit: only letters should change */
	strcpy(buf, "a1b2c3");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri mixed(\"a1b2c3\")", "A1B2C3", buf);

	/* all indices 0-based: 0,1,2,3,4 for "hello" */
	strcpy(buf, "hello");
	g_call_count = 0;
	ft_striteri(buf, record);
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

	/* empty string — must not crash */
	strcpy(buf, "");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri(\"\") unchanged", "", buf);
}
