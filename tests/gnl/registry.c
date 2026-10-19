#include "../../include/tester.h"
#include "../../include/weak_functions.h"

void	test_get_next_line(t_ctx *ctx);
void	test_gnl_lines(t_ctx *ctx);
void	test_gnl_edge(t_ctx *ctx);

static t_suite	g_suites[] = {
	{"get_next_line",  test_get_next_line, (void *)get_next_line, 1},
	{"gnl_lines",      test_gnl_lines,     (void *)get_next_line, 1},
	{"gnl_edge_cases", test_gnl_edge,      (void *)get_next_line, 1},
};

static t_project	g_project = {
	"GNL",
	g_suites,
	sizeof(g_suites) / sizeof(g_suites[0]),
	0
};

t_project	*get_gnl_project(void)
{
	return (&g_project);
}

#ifdef WITH_GNL_BONUS

void	test_gnl_two_fd(t_ctx *ctx);
void	test_gnl_three_fd(t_ctx *ctx);

static t_suite	g_bonus_suites[] = {
	{"gnl_two_fd",   test_gnl_two_fd,   (void *)get_next_line, 1},
	{"gnl_three_fd", test_gnl_three_fd, (void *)get_next_line, 1},
};

static t_project	g_bonus_project = {
	"GNL_bonus",
	g_bonus_suites,
	sizeof(g_bonus_suites) / sizeof(g_bonus_suites[0]),
	0
};

t_project	*get_gnl_bonus_project(void)
{
	return (&g_bonus_project);
}

#endif
