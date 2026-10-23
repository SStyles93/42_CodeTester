#ifndef PF_TESTER_H
# define PF_TESTER_H

# include "../../include/tester.h"
# include "../../include/weak_functions.h"
# include <limits.h>

/*
** Every case is checked against the host libc: the expected output and
** return value come from snprintf() called with the same arguments, so the
** reference is whatever the real printf prints on this machine (glibc on
** Linux, libSystem on macOS).
**
** Some cases deliberately use flag/specifier combinations the C standard
** leaves undefined (Printf_original suites), so silence the compiler's
** format checks for the test files.
*/
# pragma GCC diagnostic ignored "-Wformat"
# pragma GCC diagnostic ignored "-Wformat-extra-args"
# pragma GCC diagnostic ignored "-Wformat-zero-length"
# pragma GCC diagnostic ignored "-Wformat-security"
# ifndef __clang__
#  pragma GCC diagnostic ignored "-Wformat-truncation"
#  pragma GCC diagnostic ignored "-Wformat-overflow"
# endif

# define PF_BUF_SIZE 262144

typedef struct s_pfcap
{
	FILE	*file;
	int		saved_fd;
}	t_pfcap;

typedef struct s_pfcase
{
	t_pfcap	cap;
	int		cap_ok;
	int		exp_ret;
	int		got_ret;
	int		got_len;
}	t_pfcase;

extern char	g_pf_exp[PF_BUF_SIZE];
extern char	g_pf_got[PF_BUF_SIZE];

/* Redirect fd 1 into a temp file / restore it and read what was written. */
int		pf_capture_begin(t_pfcap *cap);
int		pf_capture_end(t_pfcap *cap, char *buf, size_t size);
void	pf_report(t_ctx *ctx, const char *label, const t_pfcase *c);
void	pf_report_alt(t_ctx *ctx, const char *label, const t_pfcase *c,
			const char *alt_exp);

/*
** PF_L(ctx, label, format, args...): run snprintf and ft_printf with the same
** arguments and compare the bytes written and the return value.
** PF(ctx, format, args...): same, the label is the call's source text.
**
** PF_L_ALT(ctx, label, alt_exp, format, args...): like PF_L but also accepts
** alt_exp as a valid output.  Use for cases where two platforms produce
** different but equally correct strings (e.g. "%p" with NULL: Linux gives
** "(nil)", macOS gives "0x0").  Whichever snprintf produces is the primary
** expected; alt_exp covers the other platform.
*/
# define PF_L(ctx, label, ...) do { \
	t_pfcase	pf_c_; \
	pf_c_.exp_ret = snprintf(g_pf_exp, PF_BUF_SIZE, __VA_ARGS__); \
	pf_c_.cap_ok = (pf_capture_begin(&pf_c_.cap) == 0); \
	pf_c_.got_ret = -1; \
	if (pf_c_.cap_ok) \
		pf_c_.got_ret = ft_printf(__VA_ARGS__); \
	pf_c_.got_len = pf_capture_end(&pf_c_.cap, g_pf_got, PF_BUF_SIZE); \
	pf_report((ctx), (label), &pf_c_); \
} while (0)

# define PF_L_ALT(ctx, label, alt_exp, ...) do { \
	t_pfcase	pf_c_; \
	pf_c_.exp_ret = snprintf(g_pf_exp, PF_BUF_SIZE, __VA_ARGS__); \
	pf_c_.cap_ok = (pf_capture_begin(&pf_c_.cap) == 0); \
	pf_c_.got_ret = -1; \
	if (pf_c_.cap_ok) \
		pf_c_.got_ret = ft_printf(__VA_ARGS__); \
	pf_c_.got_len = pf_capture_end(&pf_c_.cap, g_pf_got, PF_BUF_SIZE); \
	pf_report_alt((ctx), (label), &pf_c_, (alt_exp)); \
} while (0)

# define PF(ctx, ...) PF_L(ctx, #__VA_ARGS__, __VA_ARGS__)

#endif
