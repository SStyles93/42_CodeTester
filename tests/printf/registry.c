#include "../../include/tester.h"
#include "../../include/weak_functions.h"

/*
** Three categories:
**   Printf_mandatory: conversions cspdiuxX% without any flag, and the
**                     project requirements (Makefile, header, README...);
**   Printf_bonus:     '-0.' + minimum width, '# +', and their combinations.
**                     Only assessed if the mandatory part is perfect;
**   Printf_original:  behaviour of the original printf the subject does not
**                     require (invalid / incomplete conversions, flags that
**                     do not apply). Informational: never counted as fails.
*/

void	test_ft_printf(t_ctx *ctx);
void	test_pf_c(t_ctx *ctx);
void	test_pf_s(t_ctx *ctx);
void	test_pf_p(t_ctx *ctx);
void	test_pf_di(t_ctx *ctx);
void	test_pf_u(t_ctx *ctx);
void	test_pf_x(t_ctx *ctx);
void	test_pf_percent(t_ctx *ctx);
void	test_pf_behavior(t_ctx *ctx);
void	test_pf_project(t_ctx *ctx);
void	test_pf_minus_zero_dot(t_ctx *ctx);
void	test_pf_hash_space_plus(t_ctx *ctx);
void	test_pf_combinations(t_ctx *ctx);
void	test_pf_bonus_project(t_ctx *ctx);
void	test_pf_original(t_ctx *ctx);
void	test_pf_original_flags(t_ctx *ctx);

static t_suite	g_mandatory_suites[] = {
	{"ft_printf", test_ft_printf, (void *)ft_printf, 1},
	{"printf_c", test_pf_c, (void *)ft_printf, 1},
	{"printf_s", test_pf_s, (void *)ft_printf, 1},
	{"printf_p", test_pf_p, (void *)ft_printf, 1},
	{"printf_di", test_pf_di, (void *)ft_printf, 1},
	{"printf_u", test_pf_u, (void *)ft_printf, 1},
	{"printf_x", test_pf_x, (void *)ft_printf, 1},
	{"printf_percent", test_pf_percent, (void *)ft_printf, 1},
	{"printf_behavior", test_pf_behavior, (void *)ft_printf, 1},
	{"printf_project", test_pf_project, NULL, 0},
};

static t_suite	g_bonus_suites[] = {
	{"bonus_minus_zero_dot", test_pf_minus_zero_dot, (void *)ft_printf, 1},
	{"bonus_hash_space_plus", test_pf_hash_space_plus, (void *)ft_printf, 1},
	{"bonus_combinations", test_pf_combinations, (void *)ft_printf, 1},
	{"bonus_project", test_pf_bonus_project, NULL, 0},
};

static t_suite	g_original_suites[] = {
	{"original_conversions", test_pf_original, (void *)ft_printf, 1},
	{"original_flags", test_pf_original_flags, (void *)ft_printf, 1},
};

static t_project	g_mandatory = {
	"Printf_mandatory",
	g_mandatory_suites,
	sizeof(g_mandatory_suites) / sizeof(g_mandatory_suites[0]),
	0
};

static t_project	g_bonus = {
	"Printf_bonus",
	g_bonus_suites,
	sizeof(g_bonus_suites) / sizeof(g_bonus_suites[0]),
	0
};

static t_project	g_original = {
	"Printf_original",
	g_original_suites,
	sizeof(g_original_suites) / sizeof(g_original_suites[0]),
	1
};

t_project	*get_printf_project(void)
{
	return (&g_mandatory);
}

t_project	*get_printf_bonus_project(void)
{
	return (&g_bonus);
}

t_project	*get_printf_original_project(void)
{
	return (&g_original);
}
