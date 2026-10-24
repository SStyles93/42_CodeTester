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

/* "0,1,2" from the indices record() received. */
static const char	*join_indices(char *buf, size_t size)
{
	size_t	len;
	int		i;

	buf[0] = '\0';
	i = 0;
	while (i < g_call_count && i < 32)
	{
		len = strlen(buf);
		snprintf(buf + len, size - len, i ? ",%u" : "%u", g_indices[i]);
		i++;
	}
	return (buf);
}

void	test_ft_striteri(t_ctx *ctx)
{
	char		buf[64];
	char		idx[256];

	/* in-place edit: only letters should change */
	strcpy(buf, "a1b2c3");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri(\"a1b2c3\", to_upper)", "A1B2C3", buf);

	/* all indices 0-based: 0,1,2,3,4 for "hello" */
	strcpy(buf, "hello");
	g_call_count = 0;
	ft_striteri(buf, record);
	check_str(ctx, "striteri(\"hello\"): indices received", "0,1,2,3,4",
		join_indices(idx, sizeof(idx)));

	strcpy(buf, "");
	ft_striteri(buf, to_upper_cb);
	check_str(ctx, "striteri(\"\") unchanged", "", buf);
}
