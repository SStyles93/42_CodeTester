#include "../../include/tester.h"
#include <stdlib.h>
#include <string.h>
#include "../../include/weak_functions.h"

void	test_ft_strdup(t_ctx *ctx)
{
	char	*r;
	const char *src;

	/* empty string */
	r = ft_strdup("");
	check_str(ctx, "ft_strdup(\"\") content", "", r);
	free(r);

	/* content copied and result must be a different pointer */
	src = "hello";
	r = ft_strdup(src);
	check_str(ctx, "ft_strdup(\"hello\") content", "hello", r);
	if (r)
	{
		check_ok(ctx, "ft_strdup: result is a new allocation (!= src)",
			(void *)r != (void *)src, "returned src itself, not a copy");
		free(r);
	}
}
