#include "pf_tester.h"

/*
** Combinations the C standard leaves undefined (incomplete or unknown
** conversions, flags that do not apply to the specifier, NULL format...).
** The subject does not require any particular output for them, so these
** suites belong to the informational Printf_original category: they report
** how close you are to the host libc and never count as failures.
**
** original_conversions only uses the mandatory syntax (no flags/width);
** original_flags covers the flag cases. Cases that could crash run in
** their own process. "%s" with a NULL pointer is undefined in C (glibc
** prints "(null)", or "" when the precision is below 6).
*/

static void	isolated_no_arg(t_ctx *ctx, const void *arg)
{
	const char	*fmt;
	char		label[64];

	fmt = (const char *)arg;
	snprintf(label, sizeof(label), "\"%s\"", fmt);
	PF_L(ctx, label, fmt);
}

static void	isolated_null_format(t_ctx *ctx, const void *arg)
{
	t_pfcap	cap;
	int		r;
	int		len;

	(void)arg;
	if (pf_capture_begin(&cap) != 0)
	{
		check_ok(ctx, "ft_printf(NULL)", 0, "could not capture stdout");
		return ;
	}
	r = ft_printf(NULL);
	len = pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	check_int(ctx, "ft_printf(NULL) returns -1", -1, r);
	check_int(ctx, "ft_printf(NULL) writes nothing", 0, len);
}

static void	isolated_null_string(t_ctx *ctx, const void *arg)
{
	char	*null_str;

	(void)arg;
	null_str = NULL;
	PF(ctx, "%s", null_str);
	PF(ctx, "%s %s", null_str, "after");
}

static void	isolated_null_string_flags(t_ctx *ctx, const void *arg)
{
	(void)arg;
	PF(ctx, "%10s", (char *)NULL);
	PF(ctx, "%-10s|", (char *)NULL);
	PF(ctx, "%.5s", (char *)NULL);
	PF(ctx, "%.6s", (char *)NULL);
}

static void	run_formats(t_ctx *ctx, const char **fmts, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		run_isolated(ctx, fmts[i], isolated_no_arg, fmts[i]);
		i++;
	}
}

void	test_pf_original(t_ctx *ctx)
{
	static const char	*fmts[] = {"%", "abc%", "%%%", "%k", "a%Zb"};

	run_formats(ctx, fmts, sizeof(fmts) / sizeof(fmts[0]));
	run_isolated(ctx, "ft_printf(NULL)", isolated_null_format, NULL);
	run_isolated(ctx, "\"%s\", NULL", isolated_null_string, NULL);
}

static void	flag_mismatch(t_ctx *ctx)
{
	/* '#' on non-hex */
	PF(ctx, "%#d", 42);
	PF(ctx, "%#s", "abc");
	PF(ctx, "%#p", (void *)0x42);
	/* '+' and ' ' on unsigned / strings / chars / pointers */
	PF(ctx, "%+u", 42u);
	PF(ctx, "% x", 42u);
	PF(ctx, "%+s", "abc");
	PF(ctx, "% c", 'a');
	PF(ctx, "%+p", (void *)0x42);
	/* '0' on strings / chars / pointers */
	PF(ctx, "%05s", "abc");
	PF(ctx, "%05c", 'a');
	PF(ctx, "%020p", (void *)0x42);
	PF(ctx, "%010p", NULL);
	/* precision on chars / pointers */
	PF(ctx, "%.3c", 'a');
	PF(ctx, "%.5p", (void *)0x42);
	PF(ctx, "%.0p", NULL);
	/* combinations */
	PF(ctx, "%-0+# 10.5d|", 42);
	PF(ctx, "%-0+# 10.5x|", 42u);
}

void	test_pf_original_flags(t_ctx *ctx)
{
	static const char	*fmts[] = {"%5", "%-", "%.", "%5k", "%5%", "%-5%|",
		"%05%"};

	run_formats(ctx, fmts, sizeof(fmts) / sizeof(fmts[0]));
	flag_mismatch(ctx);
	run_isolated(ctx, "NULL string with width/precision",
		isolated_null_string_flags, NULL);
}
