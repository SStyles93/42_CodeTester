#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_strlcat(t_ctx *ctx)
{
	char	dst[20];
	size_t	ret;

	/* basic: "Hello" + " World" in size 20 → "Hello World", returns 11 */
	strcpy(dst, "Hello");
	ret = ft_strlcat(dst, " World", 20);
	check_size(ctx, "return basic (11)", 11, ret);
	check_str(ctx, "result basic", "Hello World", dst);

	/* size too small: only partial cat, return still = dst_len + src_len */
	strcpy(dst, "Hello");
	ret = ft_strlcat(dst, " World", 8);
	check_size(ctx, "truncated return (11)", 11, ret);
	check_str(ctx, "truncated result", "Hello W", dst);

	/* size <= dst_len: no write, return = size + src_len */
	strcpy(dst, "Hello");
	ret = ft_strlcat(dst, " World", 3);
	check_size(ctx, "size<dst_len return (9)", 9, ret);
	check_str(ctx, "size<dst_len dst unchanged", "Hello", dst);

	/* size=0: no write, return = size(0) + strlen(src) = 6 */
	strcpy(dst, "hello");
	ret = ft_strlcat(dst, " world", 0);
	check_size(ctx, "size=0 return (= 0+6=6)", 6, ret);
	check_str(ctx, "size=0 dst unchanged", "hello", dst);

	/* exact fit: dst_len + src_len + 1 == size (no truncation) */
	strcpy(dst, "Hello");
	ret = ft_strlcat(dst, " World", 12);
	check_size(ctx, "exact fit return (11)", 11, ret);
	check_str(ctx, "exact fit result", "Hello World", dst);
}
