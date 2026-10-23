#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

/* BSD strnstr is not always in standard headers; provide a reference impl */
static char	*ref_strnstr(const char *big, const char *little, size_t len)
{
	size_t	llen;
	size_t	i;

	if (!*little)
		return ((char *)big);
	llen = strlen(little);
	i = 0;
	while (i + llen <= len)
	{
		if (big[i] == '\0')
			return (NULL);
		if (strncmp(big + i, little, llen) == 0)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}

static void	t_strnstr(t_ctx *ctx, const char *label,
	const char *big, const char *little, size_t len)
{
	const char	*ref;
	const char	*got;

	ref = ref_strnstr(big, little, len);
	got = ft_strnstr(big, little, len);
	check_offset(ctx, label, big, ref, got);
}

void	test_ft_strnstr(t_ctx *ctx)
{
	t_strnstr(ctx, "found at start", "hello world", "hello", 11);
	t_strnstr(ctx, "found in middle", "hello world", "world", 11);
	t_strnstr(ctx, "not found", "hello world", "xyz", 11);
	t_strnstr(ctx, "empty needle (returns big)", "hello", "", 5);
	t_strnstr(ctx, "len=0", "hello", "hello", 0);
	/* needle found exactly at the boundary of len */
	t_strnstr(ctx, "needle at exact end of len", "abcdef", "ef", 6);
	/* needle crosses len by one byte: not enough room */
	t_strnstr(ctx, "needle crosses len", "abcdef", "ef", 5);
	/* needle longer than haystack but within len */
	t_strnstr(ctx, "needle longer than haystack", "hi", "hello", 5);
}
