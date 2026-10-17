#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

static int		g_del_count;
static void		*g_del_content;

static void	del_capture(void *content)
{
	g_del_content = content;
	g_del_count++;
}

void	test_ft_lstdelone(t_ctx *ctx)
{
	t_list	*n;

	/* del is called exactly once with the node's content */
	g_del_count = 0;
	g_del_content = (void *)(uintptr_t)0xBAD;
	n = ft_lstnew((void *)(uintptr_t)42);
	if (n)
	{
		ft_lstdelone(n, del_capture);
		check_int(ctx, "lstdelone: del called exactly once", 1, g_del_count);
		check_ptr(ctx, "lstdelone: del receives node's content",
			(void *)(uintptr_t)42, g_del_content);
	}

	/* NULL lst → no crash, del not called */
	g_del_count = 0;
	ft_lstdelone(NULL, del_capture);
	check_int(ctx, "lstdelone(NULL): del not called", 0, g_del_count);

	/* NULL del → no crash */
	n = ft_lstnew((void *)(uintptr_t)99);
	if (n)
	{
		ft_lstdelone(n, NULL);
		ctx->pass++;
		if (ctx->verbose)
			printf("    " GREEN "✓" RESET " lstdelone(n, NULL) didn't crash\n");
		/*
		** Per this implementation, !del causes an early return without free.
		** We must free the node ourselves to avoid a leak.
		*/
		free(n);
	}
}
