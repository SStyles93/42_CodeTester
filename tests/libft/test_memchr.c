#include "../../include/tester.h"

/*
** NOTE: Your ft_memchr.c defines the function as 'memchr' instead of
** 'ft_memchr'. Rename it to 'ft_memchr' and update libft.h accordingly.
** Until fixed, this test file will fail to link (undefined reference).
*/

#include "../../include/weak_functions.h"

static void	*ref_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p;

	p = (const unsigned char *)s;
	while (n--)
	{
		if (*p == (unsigned char)c)
			return ((void *)p);
		p++;
	}
	return (NULL);
}

static void	t_memchr(t_ctx *ctx, const char *label,
	const void *s, int c, size_t n)
{
	void		*ref;
	void		*got;
	ptrdiff_t	roff;
	ptrdiff_t	goff;

	ref = ref_memchr(s, c, n);
	got = ft_memchr(s, c, n);
	if (ref == NULL && got == NULL)
	{
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " %s (NULL)\n", label);
	}
	else if (ref == NULL || got == NULL)
	{
		ctx->fail++;
		if (ctx->verbose)
			printf("    " RED "✗" RESET " %s: expected %s, got %s\n",
				label, ref ? "ptr" : "NULL", got ? "ptr" : "NULL");
	}
	else
	{
		roff = (const unsigned char *)ref - (const unsigned char *)s;
		goff = (const unsigned char *)got - (const unsigned char *)s;
		if (roff == goff)
		{
			ctx->pass++;
			if (ctx->verbose)
				printf("    " GREEN "✓" RESET " %s (offset %ld)\n", label, (long)roff);
		}
		else
		{
			ctx->fail++;
			if (ctx->verbose)
				printf("    " RED "✗" RESET " %s: expected offset %ld, got %ld\n",
					label, (long)roff, (long)goff);
		}
	}
}

void	test_ft_memchr(t_ctx *ctx)
{
	const char		*s = "Hello, World!";
	unsigned char	bin[] = {0x00, 0x01, 0xFF, 0x42};

	t_memchr(ctx, "find 'l' at offset 2", s, 'l', 13);
	t_memchr(ctx, "find 'H' at offset 0", s, 'H', 13);
	t_memchr(ctx, "find '!'", s, '!', 13);
	t_memchr(ctx, "not found with n=5", s, 'W', 5);
	t_memchr(ctx, "found 'W' with full n", s, 'W', 13);
	t_memchr(ctx, "n=0 → NULL", s, 'H', 0);
	t_memchr(ctx, "binary: 0xFF at offset 2", bin, 0xFF, 4);
	t_memchr(ctx, "binary: 0x42 at offset 3", bin, 0x42, 4);
	t_memchr(ctx, "binary: 0x00 at offset 0", bin, 0x00, 4);
	t_memchr(ctx, "binary: not found", bin, 0xAB, 4);
	/* c is masked to unsigned char: 'a' + 256 == 353, (uchar)353 == 'a' */
	t_memchr(ctx, "c > 255 masked to 'a'", "abc", 'a' + 256, 3);
	/* find char at exactly position n-1 (last examined byte) */
	t_memchr(ctx, "find char at pos n-1", "abcde", 'e', 5);
	/* repeated chars: first occurrence returned */
	t_memchr(ctx, "repeated char finds first", "aabbc", 'b', 5);
	/* search range shorter than string: char at pos n is not found */
	t_memchr(ctx, "char just past n not found", "abcde", 'e', 4);
}
