#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_lstsize(t_ctx *ctx)
{
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;
	t_list	*n4;
	t_list	*n5;

	/* NULL → 0 */
	check_uint(ctx, "lstsize(NULL) == 0", 0, ft_lstsize(NULL));

	/* single node */
	n1 = ft_lstnew((void *)(uintptr_t)1);
	n1->next = NULL;
	check_uint(ctx, "lstsize([1]) == 1", 1, ft_lstsize(n1));

	/* two nodes */
	n2 = ft_lstnew((void *)(uintptr_t)2);
	n2->next = NULL;
	n1->next = n2;
	check_uint(ctx, "lstsize([1,2]) == 2", 2, ft_lstsize(n1));

	/* three nodes */
	n3 = ft_lstnew((void *)(uintptr_t)3);
	n3->next = NULL;
	n2->next = n3;
	check_uint(ctx, "lstsize([1,2,3]) == 3", 3, ft_lstsize(n1));

	/* five nodes */
	n4 = ft_lstnew((void *)(uintptr_t)4);
	n5 = ft_lstnew((void *)(uintptr_t)5);
	n4->next = n5;
	n5->next = NULL;
	n3->next = n4;
	check_uint(ctx, "lstsize([1..5]) == 5", 5, ft_lstsize(n1));

	/* lstsize must not modify the list structure */
	ft_lstsize(n1);
	check_ptr(ctx, "lstsize: n1->next unchanged", n2, n1->next);
	check_ptr(ctx, "lstsize: n2->next unchanged", n3, n2->next);
	check_ptr(ctx, "lstsize: n3->next unchanged", n4, n3->next);
	check_ptr(ctx, "lstsize: n4->next unchanged", n5, n4->next);
	check_ptr(ctx, "lstsize: n5->next unchanged", NULL, n5->next);

	/* mid-list: size from n3 onward == 3 */
	check_uint(ctx, "lstsize from n3 ([3,4,5]) == 3", 3, ft_lstsize(n3));

	/* size from last node == 1 */
	check_uint(ctx, "lstsize of tail only == 1", 1, ft_lstsize(n5));

	free(n1);
	free(n2);
	free(n3);
	free(n4);
	free(n5);
}
