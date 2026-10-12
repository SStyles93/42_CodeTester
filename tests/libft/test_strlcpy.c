#include "../../include/tester.h"
#include <string.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size);

void	test_ft_strlcpy(t_ctx *ctx)
{
	char	dst[20];
	size_t	ret;

	/* basic copy, enough space */
	ret = ft_strlcpy(dst, "Hello", 20);
	check_size(ctx, "return = src len (5)", 5, ret);
	check_str(ctx, "basic copy result", "Hello", dst);

	/* truncation: size smaller than src */
	ret = ft_strlcpy(dst, "Hello, World!", 6);
	check_size(ctx, "truncated return = full src len (13)", 13, ret);
	check_str(ctx, "truncated to 5 + NUL", "Hello", dst);

	/* size=1: only NUL written */
	strcpy(dst, "XXXX");
	ret = ft_strlcpy(dst, "Hello", 1);
	check_size(ctx, "size=1 return (5)", 5, ret);
	check_str(ctx, "size=1 result is empty string", "", dst);

	/* size=0: no write at all */
	strcpy(dst, "XXXX");
	ret = ft_strlcpy(dst, "Hello", 0);
	check_size(ctx, "size=0 return (5)", 5, ret);
	check_str(ctx, "size=0 dst unchanged", "XXXX", dst);

	/* empty src */
	ret = ft_strlcpy(dst, "", 10);
	check_size(ctx, "empty src return (0)", 0, ret);
	check_str(ctx, "empty src result", "", dst);

	/* exact fit: size == strlen(src) + 1 */
	{
		char	dst2[6];
		size_t	r2;
		r2 = ft_strlcpy(dst2, "Hello", 6);
		check_size(ctx, "exact fit return (5)", 5, r2);
		check_str(ctx, "exact fit result", "Hello", dst2);
	}
	/* NUL terminator present after truncation */
	{
		char	dst3[4];
		size_t	r3;
		dst3[3] = 'X';
		r3 = ft_strlcpy(dst3, "Hello", 4);
		check_size(ctx, "truncated return (5)", 5, r3);
		check_char(ctx, "NUL at size-1", '\0', dst3[3]);
		check_str(ctx, "truncated content", "Hel", dst3);
	}
	/* stress: copy 1023-char string */
	{
		static char	src2[1024];
		static char	dst4[1024];
		size_t		r4;
		memset(src2, 'Q', 1023);
		src2[1023] = '\0';
		r4 = ft_strlcpy(dst4, src2, 1024);
		check_size(ctx, "stress copy 1023 chars return", 1023, r4);
		check_mem(ctx, "stress copy 1023 chars content", src2, dst4, 1024);
	}
}
