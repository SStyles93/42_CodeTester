#include "../../include/tester.h"
#include "../../include/weak_functions.h"

void	test_ft_printf(t_ctx *ctx);

static t_suite	g_suites[] = {
	{"ft_printf", test_ft_printf, (void *)ft_printf, 1},
};

static t_project	g_project = {
	"Printf",
	g_suites,
	sizeof(g_suites) / sizeof(g_suites[0])
};

t_project	*get_printf_project(void)
{
	return (&g_project);
}
