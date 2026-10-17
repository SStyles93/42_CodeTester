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
** their own process.
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
		ctx->fail++;
		return ;
	}
	r = ft_printf(NULL);
	len = pf_capture_end(&cap, g_pf_got, PF_BUF_SIZE);
	check_int(ctx, "ft_printf(NULL) returns -1", -1, r);
	check_int(ctx, "ft_printf(NULL) writes nothing", 0, len);
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
	static const char	*fmts[] = {"%", "abc%", "%%%", "%k", "%y", "%w|",
		"a%Zb", "%!"};

	run_formats(ctx, fmts, sizeof(fmts) / sizeof(fmts[0]));
	run_isolated(ctx, "ft_printf(NULL)", isolated_null_format, NULL);
}

static void	flag_mismatch(t_ctx *ctx)
{
	/* '#' on non-hex */
	PF(ctx, "%#d", 42);
	PF(ctx, "%#i", 42);
	PF(ctx, "%#u", 42u);
	PF(ctx, "%#c", 'a');
	PF(ctx, "%#s", "abc");
	PF(ctx, "%#p", (void *)0x42);
	/* '+' and ' ' on unsigned / strings / chars / pointers */
	PF(ctx, "%+u", 42u);
	PF(ctx, "% u", 42u);
	PF(ctx, "%+x", 42u);
	PF(ctx, "% x", 42u);
	PF(ctx, "%+X", 42u);
	PF(ctx, "%+s", "abc");
	PF(ctx, "% s", "abc");
	PF(ctx, "%+c", 'a');
	PF(ctx, "% c", 'a');
	PF(ctx, "%+p", (void *)0x42);
	PF(ctx, "% p", (void *)0x42);
	PF(ctx, "%+p", NULL);
	/* '0' on strings / chars / pointers */
	PF(ctx, "%05s", "abc");
	PF(ctx, "%05c", 'a');
	PF(ctx, "%020p", (void *)0x42);
	PF(ctx, "%010p", NULL);
	PF(ctx, "%05s", (char *)NULL);
	/* precision on chars / pointers */
	PF(ctx, "%.3c", 'a');
	PF(ctx, "%.0c", 'a');
	PF(ctx, "%.5p", (void *)0x42);
	PF(ctx, "%.0p", NULL);
	PF(ctx, "%.0p", (void *)0x42);
	PF(ctx, "%20.15p", (void *)0x42);
	/* combinations */
	PF(ctx, "%-#5p|", (void *)0x42);
	PF(ctx, "%+ 05d", 42);
	PF(ctx, "%-0+# 10.5d|", 42);
	PF(ctx, "%-0+# 10.5x|", 42u);
}

void	test_pf_original_flags(t_ctx *ctx)
{
	static const char	*fmts[] = {"%5", "%-", "%.", "%#", "%+", "% ",
		"%5k", "%-5k|", "%5%", "%-5%|", "%05%", "%.3%", "% %", "%+%", "%#%"};

	run_formats(ctx, fmts, sizeof(fmts) / sizeof(fmts[0]));
	flag_mismatch(ctx);
}
