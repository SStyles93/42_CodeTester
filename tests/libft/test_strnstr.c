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
	if (ref == NULL && got == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s (both NULL)\n", label);
	}
	else if (ref == NULL || got == NULL)
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s: expected %s, got %s\n",
				label, ref ? "ptr" : "NULL", got ? "ptr" : "NULL");
	}
	else if ((ref - big) == (got - big))
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s (offset %ld)\n",
				label, (long)(ref - big));
	}
	else
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s: expected offset %ld, got %ld\n",
				label, (long)(ref - big), (long)(got - big));
	}
}

void	test_ft_strnstr(t_ctx *ctx)
{
	t_strnstr(ctx, "found at start", "hello world", "hello", 11);
	t_strnstr(ctx, "found in middle", "hello world", "world", 11);
	t_strnstr(ctx, "needle = haystack", "hello", "hello", 5);
	t_strnstr(ctx, "not found", "hello world", "xyz", 11);
	t_strnstr(ctx, "empty needle → big", "hello", "", 5);
	t_strnstr(ctx, "len=0 → NULL", "hello", "hello", 0);
	t_strnstr(ctx, "needle longer than len", "hello", "hello", 3);
	t_strnstr(ctx, "not found beyond len", "hello world", "world", 8);
	t_strnstr(ctx, "single char", "abcdef", "c", 6);
	t_strnstr(ctx, "repeated, find first", "ababab", "ab", 6);
	/* empty haystack with empty needle (len=0): empty needle → big */
	t_strnstr(ctx, "empty both, len=0", "", "", 0);
	/* needle found exactly at the boundary of len */
	t_strnstr(ctx, "needle at exact end of len", "abcdef", "ef", 6);
	/* needle starts at len-1: not enough room */
	t_strnstr(ctx, "needle starts at last byte", "abcdef", "ef", 5);
	/* single char haystack, single char needle */
	t_strnstr(ctx, "single char found", "a", "a", 1);
	t_strnstr(ctx, "single char not found", "a", "b", 1);
	/* needle longer than haystack but within len */
	t_strnstr(ctx, "needle longer than haystack", "hi", "hello", 5);
}
