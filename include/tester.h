#ifndef TESTER_H
# define TESTER_H

# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# ifndef _WIN32
#  include <termios.h>
# endif

# define RED    "\033[0;31m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define CYAN   "\033[0;36m"
# define BOLD   "\033[1m"
# define DIM    "\033[2m"
# define RESET  "\033[0m"

typedef struct s_ctx
{
	int	pass;
	int	fail;
	int	verbose;
}	t_ctx;

typedef struct s_suite
{
	const char	*name;
	void		(*run)(t_ctx *ctx);
	void		*fn_ptr;
	int			check_fn;
}	t_suite;

typedef struct s_project
{
	const char	*name;
	t_suite		*suites;
	int			suite_count;
	int			informational;	/* results shown, but never counted as fails */
}	t_project;

/* Enough for any value formatted by fmt_bytes() / fmt_str(). */
# define SHOW_BUF 640

/*
	Assertion helpers (src/utils.c). Every result is one line:
		✓ <name> → <value>
		✗ <name> → expected: <value> → got: <value>
*/
const char	*fmt_bytes(char *buf, size_t size, const char *s, size_t len);
const char	*fmt_str(char *buf, size_t size, const char *s);
void	report_pass(t_ctx *ctx, const char *label, const char *value);
void	report_fail(t_ctx *ctx, const char *label, const char *expected,
			const char *got);
void	check_ok(t_ctx *ctx, const char *label, int ok, const char *why);
void	check_offset(t_ctx *ctx, const char *label, const void *base,
			const void *expected, const void *got);
void	check_sign(t_ctx *ctx, const char *label, int expected, int got);
void	check_bool(t_ctx *ctx, const char *label, int expected, int got);
void	check_int(t_ctx *ctx, const char *label, int expected, int got);
void	check_uint(t_ctx *ctx, const char *label, unsigned int expected, unsigned int got);
void	check_size(t_ctx *ctx, const char *label, size_t expected, size_t got);
void	check_char(t_ctx *ctx, const char *label, char expected, char got);
void	check_str(t_ctx *ctx, const char *label, const char *expected, const char *got);
void	check_mem(t_ctx *ctx, const char *label, const void *expected, const void *got, size_t n);
void	check_ptr(t_ctx *ctx, const char *label, const void *expected, const void *got);
void	check_not_null(t_ctx *ctx, const char *label, const void *got);

/* Allocation tracker (src/memcheck.c) */
int		mem_available(void);
void	mem_begin(int fail_at);
int		mem_end(void);
int		mem_alloc_count(void);
int		mem_fail_hit(void);

/* read() count checker (src/readcheck.c) */
int		read_check_available(void);
void	read_check_begin(int fd, size_t expected);
int		read_check_end(void);

/* Runner (src/runner.c) */
extern int	g_test_timeout;
void	run_isolated(t_ctx *ctx, const char *label,
			void (*fn)(t_ctx *, const void *), const void *arg);

void	run_suite(t_project *project, t_suite *suite, int verbose);
void	run_project_all(t_project *project, int verbose);
void	run_suite_by_name(t_project *project, const char *name, int verbose);

/* Menu (src/menu.c) */
void	launch_menu(t_project **projects, int count);

/* Project registries */
t_project	**get_all_projects(int *count);

#endif
