#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

static char	to_upper(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

/* Records the first and last index received by the callback */
static unsigned int	g_first_idx;
static unsigned int	g_last_idx;
static int			g_calls;

static char	record_idx(unsigned int i, char c)
{
	if (g_calls == 0)
		g_first_idx = i;
	g_last_idx = i;
	g_calls++;
	return (c);
}

void	test_ft_strmapi(t_ctx *ctx)
{
	char		*r;

	/* only letters should change */
	r = ft_strmapi("abc123!!", to_upper);
	check_str(ctx, "strmapi(\"abc123!!\", to_upper)", "ABC123!!", r);
	free(r);

	/* empty string */
	r = ft_strmapi("", to_upper);
	check_str(ctx, "strmapi(\"\", to_upper)", "", r);
	free(r);

	/* index passed to callback: 0 .. strlen-1 */
	g_calls = 0;
	g_first_idx = 999;
	g_last_idx = 999;
	r = ft_strmapi("hello", record_idx);
	check_uint(ctx, "strmapi: first char index == 0", 0, g_first_idx);
	check_uint(ctx, "strmapi: last char index == 4 (strlen-1)", 4, g_last_idx);
	free(r);
}
