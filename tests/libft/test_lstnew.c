#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_lstnew(t_ctx *ctx)
{
	t_list	*n;

	/* integer-as-pointer content */
	n = ft_lstnew((void *)(uintptr_t)42);
	check_not_null(ctx, "lstnew(42) not NULL", n);
	if (n)
	{
		check_ptr(ctx, "lstnew(42) content == 42",
			(void *)(uintptr_t)42, n->content);
		check_ptr(ctx, "lstnew(42) next == NULL", NULL, n->next);
		free(n);
	}

	/* NULL content — must still allocate the node */
	n = ft_lstnew(NULL);
	check_not_null(ctx, "lstnew(NULL) not NULL", n);
	if (n)
	{
		check_ptr(ctx, "lstnew(NULL) content == NULL", NULL, n->content);
		free(n);
	}

	/* pointer-to-string content: stored verbatim, not copied */
	{
		char	*s = "libft";

		n = ft_lstnew((void *)s);
		check_not_null(ctx, "lstnew(\"libft\") not NULL", n);
		if (n)
		{
			check_ptr(ctx, "lstnew(\"libft\") content == str ptr",
				(void *)s, n->content);
			free(n);
		}
	}
}
