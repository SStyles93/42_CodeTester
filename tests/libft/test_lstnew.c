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
		check_ptr(ctx, "lstnew(NULL) next == NULL", NULL, n->next);
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
			check_ptr(ctx, "lstnew(\"libft\") next == NULL", NULL, n->next);
			free(n);
		}
	}

	/* content == 1 (minimum non-NULL integer) */
	n = ft_lstnew((void *)(uintptr_t)1);
	check_not_null(ctx, "lstnew(1) not NULL", n);
	if (n)
	{
		check_ptr(ctx, "lstnew(1) content == 1",
			(void *)(uintptr_t)1, n->content);
		check_ptr(ctx, "lstnew(1) next == NULL", NULL, n->next);
		free(n);
	}

	/* large content value */
	n = ft_lstnew((void *)(uintptr_t)0xDEADBEEFUL);
	check_not_null(ctx, "lstnew(0xDEADBEEF) not NULL", n);
	if (n)
	{
		check_ptr(ctx, "lstnew(0xDEADBEEF) content",
			(void *)(uintptr_t)0xDEADBEEFUL, n->content);
		check_ptr(ctx, "lstnew(0xDEADBEEF) next == NULL", NULL, n->next);
		free(n);
	}

	/* two consecutive nodes must be distinct allocations */
	{
		t_list	*a;
		t_list	*b;

		a = ft_lstnew((void *)(uintptr_t)1);
		b = ft_lstnew((void *)(uintptr_t)2);
		check_not_null(ctx, "lstnew x2: node a not NULL", a);
		check_not_null(ctx, "lstnew x2: node b not NULL", b);
		if (a && b)
		{
			if (a != b)
			{
				ctx->pass++;
				if (ctx->verbose)
					printf("    " GREEN "✓" RESET
						" lstnew: two nodes are distinct allocations\n");
			}
			else
			{
				ctx->fail++;
				if (ctx->verbose)
					printf("    " RED "✗" RESET
						" lstnew: two nodes share the same address\n");
			}
			check_ptr(ctx, "lstnew x2: b->next == NULL", NULL, b->next);
		}
		free(a);
		free(b);
	}

	/* malloc-fail path: returns NULL and leaks nothing (Linux memcheck only) */
	if (mem_available())
	{
		void	*result;
		int		leaked;

		mem_begin(1);
		result = ft_lstnew((void *)(uintptr_t)99);
		check_ptr(ctx, "lstnew: malloc fail → NULL", NULL, result);
		free(result);
		leaked = mem_end();
		check_int(ctx, "lstnew: malloc fail → no leak", 0, leaked);
	}
	else
	{
		printf("    " YELLOW "!" RESET
			" lstnew malloc-fail check needs Linux wrapper — skipped\n");
	}
}
