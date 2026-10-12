#include "../../include/tester.h"
#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstnew(void *content);
void	ft_lstclear(t_list **lst, void (*del)(void *));

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
	ctx->pass++;
	if (ctx->verbose)
		printf("    " GREEN "✓" RESET " lstclear(&NULL) didn't crash\n");
	check_ptr(ctx, "lstclear(&NULL): *lst still NULL", NULL, list);
	check_int(ctx, "lstclear(&NULL): del not called", 0, g_del_count);

	/* single node: *lst becomes NULL, del called once */
	g_del_count = 0;
	n1 = ft_lstnew((void *)(uintptr_t)1);
	list = n1;
	ft_lstclear(&list, del_noop);
	check_ptr(ctx, "lstclear([1]): *lst == NULL", NULL, list);
	check_int(ctx, "lstclear([1]): del called once", 1, g_del_count);

	/* three nodes: *lst becomes NULL, del called 3 times */
	g_del_count = 0;
	n1 = ft_lstnew((void *)(uintptr_t)1);
	n2 = ft_lstnew((void *)(uintptr_t)2);
	n3 = ft_lstnew((void *)(uintptr_t)3);
	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;
	list = n1;
	ft_lstclear(&list, del_noop);
	check_ptr(ctx, "lstclear([1,2,3]): *lst == NULL", NULL, list);
	check_int(ctx, "lstclear([1,2,3]): del called 3 times", 3, g_del_count);

	/* calling clear again on now-NULL list: idempotent, del not called */
	g_del_count = 0;
	ft_lstclear(&list, del_noop);
	check_ptr(ctx, "lstclear again on NULL: *lst still NULL", NULL, list);
	check_int(ctx, "lstclear again on NULL: del not called", 0, g_del_count);

	/* heap-allocated content: del must free it, no leaks */
	g_del_count = 0;
	{
		char	*s1 = (char *)malloc(4);
		char	*s2 = (char *)malloc(4);

		if (s1 && s2)
		{
			s1[0] = 'a'; s1[1] = '\0';
			s2[0] = 'b'; s2[1] = '\0';
			n1 = ft_lstnew((void *)s1);
			n2 = ft_lstnew((void *)s2);
			if (n1 && n2)
			{
				n1->next = n2;
				n2->next = NULL;
				list = n1;
				ft_lstclear(&list, del_free_count);
				check_ptr(ctx, "lstclear with heap content: *lst == NULL",
					NULL, list);
				check_int(ctx, "lstclear with heap content: del called twice",
					2, g_del_count);
			}
			else
			{
				free(s1);
				free(s2);
				free(n1);
				free(n2);
			}
		}
	}
}
