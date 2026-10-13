#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_lstlast(t_ctx *ctx)
{
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;
	t_list	*last;

	/* NULL → NULL */
	check_ptr(ctx, "lstlast(NULL) == NULL", NULL, ft_lstlast(NULL));

	/* single node is both head and tail */
	n1 = ft_lstnew((void *)(uintptr_t)1);
	n1->next = NULL;
	last = ft_lstlast(n1);
	check_ptr(ctx, "lstlast([1]) == n1", n1, last);
	if (last)
		check_ptr(ctx, "lstlast([1])->next == NULL", NULL, last->next);

	/* two nodes: last is n2 */
	n2 = ft_lstnew((void *)(uintptr_t)2);
	n2->next = NULL;
	n1->next = n2;
	last = ft_lstlast(n1);
	check_ptr(ctx, "lstlast([1,2]) == n2", n2, last);
	if (last)
	{
		check_ptr(ctx, "lstlast([1,2])->next == NULL", NULL, last->next);
		check_ptr(ctx, "lstlast([1,2]) content == 2",
			(void *)(uintptr_t)2, last->content);
	}

	/* three nodes: last is n3 */
	n3 = ft_lstnew((void *)(uintptr_t)3);
	n3->next = NULL;
	n2->next = n3;
	last = ft_lstlast(n1);
	check_ptr(ctx, "lstlast([1,2,3]) == n3", n3, last);
	if (last)
	{
		check_ptr(ctx, "lstlast([1,2,3])->next == NULL", NULL, last->next);
		check_ptr(ctx, "lstlast([1,2,3]) content == 3",
			(void *)(uintptr_t)3, last->content);
	}

	/* lstlast must not modify the list */
	ft_lstlast(n1);
	check_ptr(ctx, "lstlast: n1->next unchanged", n2, n1->next);
	check_ptr(ctx, "lstlast: n2->next unchanged", n3, n2->next);
	check_ptr(ctx, "lstlast: n3->next still NULL", NULL, n3->next);

	/* calling lstlast from a mid-list node returns correct last */
	last = ft_lstlast(n2);
	check_ptr(ctx, "lstlast from n2 == n3", n3, last);

	/* last node reports itself */
	last = ft_lstlast(n3);
	check_ptr(ctx, "lstlast(n3) == n3", n3, last);

	free(n1);
	free(n2);
	free(n3);
}
