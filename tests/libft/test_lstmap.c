#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

static int	g_del_count;

static void	del_noop(void *content)
{
	(void)content;
	g_del_count++;
}

static void	*map_add1(void *content)
{
	return ((void *)((uintptr_t)content + 1));
}

static void	*map_double(void *content)
{
	return ((void *)((uintptr_t)content * 2));
}

void	test_ft_lstmap(t_ctx *ctx)
{
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;
	t_list	*src;
	t_list	*mapped;

	/* NULL lst → NULL */
	mapped = ft_lstmap(NULL, map_add1, del_noop);
	check_ptr(ctx, "lstmap(NULL, f, del) == NULL", NULL, mapped);

	/* three-node mapping: 1→2→3 doubled to 2→4→6 */
	n1 = ft_lstnew((void *)(uintptr_t)1);
	n2 = ft_lstnew((void *)(uintptr_t)2);
	n3 = ft_lstnew((void *)(uintptr_t)3);
	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;
	src = n1;
	g_del_count = 0;
	mapped = ft_lstmap(src, map_double, del_noop);
	check_not_null(ctx, "lstmap([1,2,3], *2): result not NULL", mapped);
	if (mapped)
	{
		check_uint(ctx, "lstmap([1,2,3], *2): size == 3",
			3, ft_lstsize(mapped));
		check_ptr(ctx, "lstmap: [0].content == 2",
			(void *)(uintptr_t)2, mapped->content);
		if (mapped->next)
			check_ptr(ctx, "lstmap: [1].content == 4",
				(void *)(uintptr_t)4, mapped->next->content);
		if (mapped->next && mapped->next->next)
		{
			check_ptr(ctx, "lstmap: [2].content == 6",
				(void *)(uintptr_t)6, mapped->next->next->content);
			check_ptr(ctx, "lstmap: [2].next == NULL",
				NULL, mapped->next->next->next);
		}
		/* result nodes must be distinct from source nodes */
		check_ok(ctx, "lstmap: result is a new list (!= src)", mapped != src,
			"returned the source list itself");
		ft_lstclear(&mapped, del_noop);
	}
	/* original list must be fully intact after mapping */
	check_ptr(ctx, "lstmap: original [0].content == 1",
		(void *)(uintptr_t)1, n1->content);
	check_ptr(ctx, "lstmap: original [1].content == 2",
		(void *)(uintptr_t)2, n2->content);
	check_ptr(ctx, "lstmap: original [2].content == 3",
		(void *)(uintptr_t)3, n3->content);
	check_ptr(ctx, "lstmap: original n1->next == n2", n2, n1->next);
	check_ptr(ctx, "lstmap: original n2->next == n3", n3, n2->next);
	check_ptr(ctx, "lstmap: original n3->next == NULL", NULL, n3->next);
	free(n1);
	free(n2);
	free(n3);
}
