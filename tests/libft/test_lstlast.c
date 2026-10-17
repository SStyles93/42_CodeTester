#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

void	test_ft_lstlast(t_ctx *ctx)
{
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;

	/* NULL → NULL */
	check_ptr(ctx, "lstlast(NULL) == NULL", NULL, ft_lstlast(NULL));

	/* single node is both head and tail */
	n1 = ft_lstnew((void *)(uintptr_t)1);
	n1->next = NULL;
	check_ptr(ctx, "lstlast([1]) == n1", n1, ft_lstlast(n1));

	/* three nodes: last is n3 */
	n2 = ft_lstnew((void *)(uintptr_t)2);
	n3 = ft_lstnew((void *)(uintptr_t)3);
	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;
	check_ptr(ctx, "lstlast([1,2,3]) == n3", n3, ft_lstlast(n1));

	free(n1);
	free(n2);
	free(n3);
}
