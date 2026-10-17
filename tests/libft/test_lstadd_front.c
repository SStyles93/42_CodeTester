#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_lstadd_front(t_ctx *ctx)
{
	t_list	*list;
	t_list	*n1;
	t_list	*n2;

	/* add to empty list: new node becomes head, its stale next is reset */
	list = NULL;
	n1 = ft_lstnew((void *)(uintptr_t)10);
	n1->next = (t_list *)(uintptr_t)0xBAD;
	ft_lstadd_front(&list, n1);
	check_ptr(ctx, "lstadd_front on empty: head == n1", n1, list);
	check_ptr(ctx, "lstadd_front on empty: pre-set next overwritten to NULL",
		NULL, n1->next);

	/* add second node: new head, old head becomes n->next */
	n2 = ft_lstnew((void *)(uintptr_t)20);
	ft_lstadd_front(&list, n2);
	check_ptr(ctx, "lstadd_front 2nd: head == n2", n2, list);
	check_ptr(ctx, "lstadd_front 2nd: n2->next == n1", n1, n2->next);
	check_ptr(ctx, "lstadd_front 2nd: n1->next == NULL", NULL, n1->next);

	free(n1);
	free(n2);
}
