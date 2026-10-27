#include "gnl_tester.h"

/*
Two categories:
	GNL_mandatory:
				get_next_line.c / get_next_line_utils.c, and the project
				requirements (files, compilation, functions, README...);
	GNL_bonus:
				get_next_line_bonus.c / get_next_line_utils_bonus.c: the
				same suites again (the bonus must still be a correct
				get_next_line), several fds at once and one static.
				Only assessed if the mandatory part is perfect.

The bonus get_next_line is compiled as get_next_line_bonus (see Makefile),
so both implementations are linked and tested independently.
*/

void	test_get_next_line(t_ctx *ctx);
void	test_gnl_lines(t_ctx *ctx);
void	test_gnl_edge(t_ctx *ctx);
void	test_gnl_sizes(t_ctx *ctx);
void	test_gnl_big_file(t_ctx *ctx);
void	test_gnl_read_little(t_ctx *ctx);
void	test_gnl_read_count(t_ctx *ctx);
void	test_gnl_sources(t_ctx *ctx);
void	test_gnl_memory(t_ctx *ctx);
void	test_gnl_project(t_ctx *ctx);
void	test_gnl_two_fd(t_ctx *ctx);
void	test_gnl_three_fd(t_ctx *ctx);
void	test_gnl_bonus_project(t_ctx *ctx);

/* m_<suite> runs it on the mandatory get_next_line, b_<suite> on the bonus. */
#define GNL_WRAP_BONUS(suite) \
	static void	b_##suite(t_ctx *ctx) \
	{ \
		g_gnl = get_next_line_bonus; \
		suite(ctx); \
	}
#define GNL_WRAP(suite) \
	static void	m_##suite(t_ctx *ctx) \
	{ \
		g_gnl = get_next_line; \
		suite(ctx); \
	} \
	GNL_WRAP_BONUS(suite)

GNL_WRAP(test_get_next_line)
GNL_WRAP(test_gnl_lines)
GNL_WRAP(test_gnl_edge)
GNL_WRAP(test_gnl_sizes)
GNL_WRAP(test_gnl_big_file)
GNL_WRAP(test_gnl_read_little)
GNL_WRAP(test_gnl_read_count)
GNL_WRAP(test_gnl_sources)
GNL_WRAP(test_gnl_memory)
GNL_WRAP_BONUS(test_gnl_two_fd)
GNL_WRAP_BONUS(test_gnl_three_fd)

#define MAN(name, suite) {name, m_##suite, (void *)get_next_line, 1}
#define BON(name, suite) {name, b_##suite, (void *)get_next_line_bonus, 1}

static t_suite	g_mandatory_suites[] = {
	MAN("get_next_line", test_get_next_line),
	MAN("gnl_lines", test_gnl_lines),
	MAN("gnl_edge_cases", test_gnl_edge),
	MAN("gnl_buffer_size", test_gnl_sizes),
	MAN("gnl_big_file", test_gnl_big_file),
	MAN("gnl_read_little", test_gnl_read_little),
	MAN("gnl_read_count", test_gnl_read_count),
	MAN("gnl_sources", test_gnl_sources),
	MAN("gnl_memory", test_gnl_memory),
	{"gnl_project", test_gnl_project, NULL, 0},
};

static t_suite	g_bonus_suites[] = {
	BON("bonus_get_next_line", test_get_next_line),
	BON("bonus_lines", test_gnl_lines),
	BON("bonus_edge_cases", test_gnl_edge),
	BON("bonus_buffer_size", test_gnl_sizes),
	BON("bonus_big_file", test_gnl_big_file),
	BON("bonus_read_little", test_gnl_read_little),
	BON("bonus_read_count", test_gnl_read_count),
	BON("bonus_sources", test_gnl_sources),
	BON("bonus_memory", test_gnl_memory),
	BON("bonus_two_fd", test_gnl_two_fd),
	BON("bonus_three_fd", test_gnl_three_fd),
	{"bonus_project", test_gnl_bonus_project, NULL, 0},
};

static t_project	g_mandatory = {
	"GNL_mandatory",
	g_mandatory_suites,
	sizeof(g_mandatory_suites) / sizeof(g_mandatory_suites[0]),
	0
};

static t_project	g_bonus = {
	"GNL_bonus",
	g_bonus_suites,
	sizeof(g_bonus_suites) / sizeof(g_bonus_suites[0]),
	0
};

t_project	*get_gnl_project(void)
{
	return (&g_mandatory);
}

t_project	*get_gnl_bonus_project(void)
{
	return (&g_bonus);
}
