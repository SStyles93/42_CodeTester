#include "../../include/tester.h"
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_strlcpy(t_ctx *ctx)
{
	char	dst[20];
	size_t	ret;

	/* basic copy, enough space */
	ret = ft_strlcpy(dst, "Hello", 20);
	check_size(ctx, "return = src len (5)", 5, ret);
	check_str(ctx, "basic copy result", "Hello", dst);

	/* exact fit: size == strlen(src) + 1 */
	ret = ft_strlcpy(dst, "Hello", 6);
	check_size(ctx, "exact fit return (5)", 5, ret);
	check_str(ctx, "exact fit result", "Hello", dst);

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
}
