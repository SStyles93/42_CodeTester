#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_lstsize(t_ctx *ctx)
{
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;

	/* NULL → 0 */
	check_uint(ctx, "lstsize(NULL) == 0", 0, ft_lstsize(NULL));

	/* single node */
	n1 = ft_lstnew((void *)(uintptr_t)1);
	n1->next = NULL;
	check_uint(ctx, "lstsize([1]) == 1", 1, ft_lstsize(n1));

	/* three nodes */
	n2 = ft_lstnew((void *)(uintptr_t)2);
	n3 = ft_lstnew((void *)(uintptr_t)3);
	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;
	check_uint(ctx, "lstsize([1,2,3]) == 3", 3, ft_lstsize(n1));

	free(n1);
	free(n2);
	free(n3);
}
