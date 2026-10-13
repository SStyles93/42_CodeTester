#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_lstadd_back(t_ctx *ctx)
{
	t_list	*list;
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;

	/* add to empty list (*lst is NULL): new node becomes the list */
	list = NULL;
	n1 = ft_lstnew((void *)(uintptr_t)10);
	ft_lstadd_back(&list, n1);
	check_ptr(ctx, "lstadd_back on empty: head == n1", n1, list);
	check_ptr(ctx, "lstadd_back on empty: n1->next == NULL", NULL, n1->next);

	/* add second: head unchanged, n2 appended at tail */
	n2 = ft_lstnew((void *)(uintptr_t)20);
	ft_lstadd_back(&list, n2);
	check_ptr(ctx, "lstadd_back 2nd: head still n1", n1, list);
	check_ptr(ctx, "lstadd_back 2nd: n1->next == n2", n2, n1->next);
	check_ptr(ctx, "lstadd_back 2nd: n2->next == NULL", NULL, n2->next);

	/* add third: FIFO order 10 → 20 → 30 */
	n3 = ft_lstnew((void *)(uintptr_t)30);
	ft_lstadd_back(&list, n3);
	check_ptr(ctx, "lstadd_back 3rd: head still n1", n1, list);
	check_ptr(ctx, "lstadd_back 3rd: n1->next == n2", n2, n1->next);
	check_ptr(ctx, "lstadd_back 3rd: n2->next == n3", n3, n2->next);
	check_ptr(ctx, "lstadd_back 3rd: n3->next == NULL", NULL, n3->next);

	/* verify content order via list traversal */
	check_ptr(ctx, "lstadd_back: [0].content == 10",
		(void *)(uintptr_t)10, list->content);
	check_ptr(ctx, "lstadd_back: [1].content == 20",
		(void *)(uintptr_t)20, list->next->content);
	check_ptr(ctx, "lstadd_back: [2].content == 30",
		(void *)(uintptr_t)30, list->next->next->content);

	free(n1);
	free(n2);
	free(n3);

	/* single-node list: second add should link via n1->next */
	list = NULL;
	n1 = ft_lstnew((void *)(uintptr_t)1);
	n2 = ft_lstnew((void *)(uintptr_t)2);
	ft_lstadd_back(&list, n1);
	ft_lstadd_back(&list, n2);
	check_ptr(ctx, "lstadd_back: n1 == head after two adds", n1, list);
	check_ptr(ctx, "lstadd_back: n2 == tail after two adds", n2, list->next);
	check_ptr(ctx, "lstadd_back: tail->next == NULL", NULL, n2->next);
	free(n1);
	free(n2);
}
