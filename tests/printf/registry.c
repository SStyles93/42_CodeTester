#include "../../include/tester.h"

void	test_ft_xyz(t_ctx *ctx);


static t_suite	g_suites[] = {
	{"ft_xyz",      test_ft_xyz},
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
