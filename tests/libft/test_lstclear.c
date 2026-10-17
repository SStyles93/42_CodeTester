#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

static int	g_del_count;

static void	del_noop(void *content)
{
	(void)content;
	g_del_count++;
}

static void	del_free_count(void *content)
{
	free(content);
	g_del_count++;
}

void	test_ft_lstclear(t_ctx *ctx)
{
	t_list	*list;
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;

	/* *lst == NULL → no crash, *lst stays NULL */
	list = NULL;
	g_del_count = 0;
	ft_lstclear(&list, del_noop);
	check_ptr(ctx, "lstclear(&NULL): *lst still NULL", NULL, list);
	check_int(ctx, "lstclear(&NULL): del not called", 0, g_del_count);

	/* three nodes with heap content: *lst becomes NULL, del frees each */
	g_del_count = 0;
	{
		char	*s1 = (char *)malloc(4);
		char	*s2 = (char *)malloc(4);
		char	*s3 = (char *)malloc(4);

		n1 = ft_lstnew((void *)s1);
		n2 = ft_lstnew((void *)s2);
		n3 = ft_lstnew((void *)s3);
		if (s1 && s2 && s3 && n1 && n2 && n3)
		{
			n1->next = n2;
			n2->next = n3;
			n3->next = NULL;
			list = n1;
			ft_lstclear(&list, del_free_count);
			check_ptr(ctx, "lstclear([1,2,3]): *lst == NULL", NULL, list);
			check_int(ctx, "lstclear([1,2,3]): del called 3 times",
				3, g_del_count);
		}
		else
		{
			free(s1);
			free(s2);
			free(s3);
			free(n1);
			free(n2);
			free(n3);
		}
	}
}
