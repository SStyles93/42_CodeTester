#include "../../include/tester.h"

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

	ref = ref_memchr(s, c, n);
	got = ft_memchr(s, c, n);
	check_offset(ctx, label, s, ref, got);
}

void	test_ft_memchr(t_ctx *ctx)
{
	const char		*s = "Hello, World!";
	unsigned char	bin[] = {0x00, 0x01, 0xFF, 0x42};

	/* repeated char: first occurrence returned */
	t_memchr(ctx, "find first 'l' at offset 2", s, 'l', 13);
	t_memchr(ctx, "find 'H' at offset 0", s, 'H', 13);
	/* find char at exactly position n-1 (last examined byte) */
	t_memchr(ctx, "find char at pos n-1", "abcde", 'e', 5);
	/* search range shorter than string: char at pos n is not found */
	t_memchr(ctx, "char just past n not found", "abcde", 'e', 4);
	t_memchr(ctx, "n=0", s, 'H', 0);
	t_memchr(ctx, "binary: 0x00 at offset 0", bin, 0x00, 4);
	t_memchr(ctx, "binary: 0xFF at offset 2", bin, 0xFF, 4);
	t_memchr(ctx, "binary: not found", bin, 0xAB, 4);
	/* c is masked to unsigned char: 'a' + 256 == 353, (uchar)353 == 'a' */
	t_memchr(ctx, "c > 255 masked to 'a'", "abc", 'a' + 256, 3);
}
