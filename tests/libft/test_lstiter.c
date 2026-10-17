#include "../../include/tester.h"
#include <stdlib.h>
#include "../../include/weak_functions.h"

static int			g_iter_count;
static uintptr_t	g_iter_vals[16];

static void	f_capture(void *content)
{
	if (g_iter_count < 16)
		g_iter_vals[g_iter_count] = (uintptr_t)content;
	g_iter_count++;
}

void	test_ft_lstiter(t_ctx *ctx)
{
	t_list	*n1;
	t_list	*n2;
	t_list	*n3;

	/* NULL list → f not called, no crash */
	g_iter_count = 0;
	ft_lstiter(NULL, f_capture);
	check_int(ctx, "lstiter(NULL): f not called", 0, g_iter_count);

	/* three nodes: f called 3 times in FIFO order 1→2→3 */
	g_iter_count = 0;
	n1 = ft_lstnew((void *)(uintptr_t)1);
	n2 = ft_lstnew((void *)(uintptr_t)2);
	n3 = ft_lstnew((void *)(uintptr_t)3);
	n1->next = n2;
	n2->next = n3;
	n3->next = NULL;
	ft_lstiter(n1, f_capture);
	check_int(ctx, "lstiter([1,2,3]): f called 3 times", 3, g_iter_count);
	if (g_iter_count >= 3)
	{
		check_ptr(ctx, "lstiter: 1st arg == 1",
			(void *)(uintptr_t)1, (void *)g_iter_vals[0]);
		check_ptr(ctx, "lstiter: 2nd arg == 2",
			(void *)(uintptr_t)2, (void *)g_iter_vals[1]);
		check_ptr(ctx, "lstiter: 3rd arg == 3",
			(void *)(uintptr_t)3, (void *)g_iter_vals[2]);
	}

	/* lstiter must not modify list structure */
	check_ptr(ctx, "lstiter: n1->next unchanged", n2, n1->next);
	check_ptr(ctx, "lstiter: n2->next unchanged", n3, n2->next);
	check_ptr(ctx, "lstiter: n3->next == NULL", NULL, n3->next);

	free(n1);
	free(n2);
	free(n3);
}
