#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

/*
** NOTE: ft_strlcat.c calls strlen() without #include <string.h>.
** Add that include to ft_strlcat.c or it will fail to compile with -Werror.
** Also: ft_strlcat uses unsigned int instead of size_t (non-standard).
*/

void	test_ft_strlcat(t_ctx *ctx)
{
	char	dst[20];
	unsigned int	ret;

	/* basic: "Hello" + " World" in size 20 → "Hello World", returns 11 */
	strcpy(dst, "Hello");
	ret = ft_strlcat(dst, " World", 20);
	check_uint(ctx, "return basic (11)", 11, ret);
	check_str(ctx, "result basic", "Hello World", dst);

	/* size too small: only partial cat, return still = dst_len + src_len */
	strcpy(dst, "Hello");
	ret = ft_strlcat(dst, " World", 8);
	check_uint(ctx, "truncated return (11)", 11, ret);
	check_str(ctx, "truncated result", "Hello W", dst);

	/* size <= dst_len: no write, return = size + src_len */
	strcpy(dst, "Hello");
	ret = ft_strlcat(dst, " World", 3);
	check_uint(ctx, "size<dst_len return (9)", 9, ret);
	check_str(ctx, "size<dst_len dst unchanged", "Hello", dst);

	/* empty dst */
	dst[0] = '\0';
	ret = ft_strlcat(dst, "hello", 6);
	check_uint(ctx, "empty dst return (5)", 5, ret);
	check_str(ctx, "empty dst result", "hello", dst);

	/* empty src */
	strcpy(dst, "hello");
	ret = ft_strlcat(dst, "", 10);
	check_uint(ctx, "empty src return (5)", 5, ret);
	check_str(ctx, "empty src dst unchanged", "hello", dst);

	/* size=0: no write, return = size(0) + strlen(src) = 6 */
	strcpy(dst, "hello");
	ret = ft_strlcat(dst, " world", 0);
	check_uint(ctx, "size=0 return (= 0+6=6)", 6, ret);
	check_str(ctx, "size=0 dst unchanged", "hello", dst);

	/* exact fit: dst_len + src_len + 1 == size (no truncation) */
	strcpy(dst, "Hello");
	ret = ft_strlcat(dst, " World", 12);
	check_uint(ctx, "exact fit return (11)", 11, ret);
	check_str(ctx, "exact fit result", "Hello World", dst);

	/* NUL terminator placed at size-1 after truncation */
	{
		char	buf2[8];
		strcpy(buf2, "Hi");
		ft_strlcat(buf2, "XXXXXXXX", 6);
		check_char(ctx, "NUL at size-1 after trunc", '\0', buf2[5]);
	}
}
