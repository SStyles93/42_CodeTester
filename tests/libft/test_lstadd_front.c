#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_lstadd_front(t_ctx *ctx)
{
	t_list	*list;
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;

	/* add to empty list: new node becomes head */
	list = NULL;
	n1 = ft_lstnew((void *)(uintptr_t)10);
	ft_lstadd_front(&list, n1);
	check_ptr(ctx, "lstadd_front on empty: head == n1", n1, list);
	check_ptr(ctx, "lstadd_front on empty: n1->next == NULL", NULL, n1->next);

	/* add second node: new head, old head becomes n->next */
	n2 = ft_lstnew((void *)(uintptr_t)20);
	ft_lstadd_front(&list, n2);
	check_ptr(ctx, "lstadd_front 2nd: head == n2", n2, list);
	check_ptr(ctx, "lstadd_front 2nd: n2->next == n1", n1, n2->next);
	check_ptr(ctx, "lstadd_front 2nd: n1->next == NULL", NULL, n1->next);

	/* add third node: LIFO order 30 → 20 → 10 */
	n3 = ft_lstnew((void *)(uintptr_t)30);
	ft_lstadd_front(&list, n3);
	check_ptr(ctx, "lstadd_front 3rd: head == n3", n3, list);
	check_ptr(ctx, "lstadd_front 3rd: n3->next == n2", n2, n3->next);
	check_ptr(ctx, "lstadd_front 3rd: n2->next == n1", n1, n2->next);
	check_ptr(ctx, "lstadd_front 3rd: n1->next == NULL", NULL, n1->next);

	/* verify content order via list traversal */
	check_ptr(ctx, "lstadd_front: [0].content == 30",
		(void *)(uintptr_t)30, list->content);
	check_ptr(ctx, "lstadd_front: [1].content == 20",
		(void *)(uintptr_t)20, list->next->content);
	check_ptr(ctx, "lstadd_front: [2].content == 10",
		(void *)(uintptr_t)10, list->next->next->content);
	check_ptr(ctx, "lstadd_front: [2].next == NULL",
		NULL, list->next->next->next);

	free(n1);
	free(n2);
	free(n3);

	/* add single node to empty: node's prior next must not matter */
	list = NULL;
	n1 = ft_lstnew((void *)(uintptr_t)99);
	n1->next = (t_list *)(uintptr_t)0xBAD;
	ft_lstadd_front(&list, n1);
	/* lstadd_front sets new->next = *lst (which was NULL) */
	check_ptr(ctx, "lstadd_front: pre-set next overwritten to NULL",
		NULL, n1->next);
	check_ptr(ctx, "lstadd_front: list head updated", n1, list);
	free(n1);
}
