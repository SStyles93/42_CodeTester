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
}
