#include "pf_tester.h"

/*
** Every valid combination of flags x width x precision for each specifier,
** against a set of edge values (about 20 000 cases). Only flags the C
** standard defines for the specifier are combined; the undefined ones live
** in the printf_undefined suite. In verbose mode only failures are printed.
*/

static const char	*g_widths[] = {"", "1", "3", "8", "15"};
static const char	*g_precs[] = {"", ".", ".0", ".1", ".3", ".8", ".15"};

#define N_WIDTHS 5
#define N_PRECS 7

static void	build_fmt(char *fmt, const char *flags, int mask, int w, int p,
	char spec)
{
	char	f[8];
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (flags[i])
	{
		if (mask & (1 << i))
			f[j++] = flags[i];
		i++;
	}
	f[j] = '\0';
	snprintf(fmt, 32, "%%%s%s%s%c", f, g_widths[w], p < 0 ? "" : g_precs[p],
		spec);
}

static void	combo_signed(t_ctx *ctx, char spec)
{
	static const int	vals[] = {0, 1, -1, 7, -7, 42, -42, 100, 99999,
		1234567, -1234567, INT_MAX, INT_MIN};
	char				fmt[32];
	char				label[64];
	int					m;
	int					w;
	int					p;
	size_t				v;

	m = -1;
	while (++m < 16)
	{
		w = -1;
		while (++w < N_WIDTHS)
		{
			p = -1;
			while (++p < N_PRECS)
			{
				build_fmt(fmt, "-0+ ", m, w, p, spec);
				v = 0;
				while (v < sizeof(vals) / sizeof(vals[0]))
				{
					snprintf(label, sizeof(label), "\"%s\", %d", fmt, vals[v]);
					PF_L(ctx, label, fmt, vals[v]);
					v++;
				}
			}
		}
	}
}

static void	combo_unsigned(t_ctx *ctx, char spec, const char *flags)
{
	static const unsigned int	vals[] = {0u, 1u, 7u, 42u, 255u, 0xabcdefu,
		(unsigned int)INT_MAX + 1u, UINT_MAX};
	char						fmt[32];
	char						label[64];
	int							m;
	int							w;
	int							p;
	size_t						v;

	m = -1;
	while (++m < (1 << (int)strlen(flags)))
	{
		w = -1;
		while (++w < N_WIDTHS)
		{
			p = -1;
			while (++p < N_PRECS)
			{
				build_fmt(fmt, flags, m, w, p, spec);
				v = 0;
				while (v < sizeof(vals) / sizeof(vals[0]))
				{
					snprintf(label, sizeof(label), "\"%s\", %u", fmt, vals[v]);
					PF_L(ctx, label, fmt, vals[v]);
					v++;
				}
			}
		}
	}
}

static void	combo_s(t_ctx *ctx)
{
	static const char	*vals[] = {"", "a", "hello", "hello world, long one",
		NULL};
	char				fmt[32];
	char				label[96];
	int					m;
	int					w;
	int					p;
	size_t				v;

	m = -1;
	while (++m < 2)
	{
		w = -1;
		while (++w < N_WIDTHS)
		{
			p = -1;
			while (++p < N_PRECS)
			{
				build_fmt(fmt, "-", m, w, p, 's');
				v = 0;
				while (v < sizeof(vals) / sizeof(vals[0]))
				{
					snprintf(label, sizeof(label), "\"%s\", \"%s\"", fmt,
						vals[v] ? vals[v] : "NULL");
					PF_L(ctx, label, fmt, vals[v]);
					v++;
				}
			}
		}
	}
}

static void	combo_c_p(t_ctx *ctx)
{
	static const int	chars[] = {'a', '0', ' ', '\0', 127, 255};
	void				*ptrs[5];
	char				fmt[32];
	char				label[64];
	int					m;
	int					w;
	size_t				v;

	ptrs[0] = NULL;
	ptrs[1] = (void *)1;
	ptrs[2] = (void *)0xabc;
	ptrs[3] = (void *)-1;
	ptrs[4] = (void *)&m;
	m = -1;
	while (++m < 2)
	{
		w = -1;
		while (++w < N_WIDTHS)
		{
			build_fmt(fmt, "-", m, w, -1, 'c');
			v = -1;
			while (++v < sizeof(chars) / sizeof(chars[0]))
			{
				snprintf(label, sizeof(label), "\"%s\", %d", fmt, chars[v]);
				PF_L(ctx, label, fmt, chars[v]);
			}
			build_fmt(fmt, "-", m, w, -1, 'p');
			v = -1;
			while (++v < sizeof(ptrs) / sizeof(ptrs[0]))
			{
				snprintf(label, sizeof(label), "\"%s\", %p", fmt, ptrs[v]);
				PF_L(ctx, label, fmt, ptrs[v]);
			}
		}
	}
}

/* Flags, width and precision must not leak into the next conversion/call. */
static void	no_state_between_calls(t_ctx *ctx)
{
	PF(ctx, "%-10d|", 1);
	PF(ctx, "%d|", 2);
	PF(ctx, "%08.3d", 3);
	PF(ctx, "%d", 4);
	PF(ctx, "%#x", 5u);
	PF(ctx, "%x", 5u);
	PF(ctx, "%+d", 6);
	PF(ctx, "%d", 6);
	PF(ctx, "% d", 7);
	PF(ctx, "%d", 7);
	PF(ctx, "%.2s", "eight");
	PF(ctx, "%s", "eight");
	PF(ctx, "%5c", '9');
	PF(ctx, "%c", '9');
	PF(ctx, "%-5d|%d|%05d|%d|%#x|%x|%+d|%d|%.1s|%s", 1, 2, 3, 4, 5u, 6u,
		7, 8, "nine", "ten");
}

void	test_pf_combinations(t_ctx *ctx)
{
	no_state_between_calls(ctx);
	g_pf_quiet = 1;
	combo_signed(ctx, 'd');
	combo_signed(ctx, 'i');
	combo_unsigned(ctx, 'u', "-0");
	combo_unsigned(ctx, 'x', "-0#");
	combo_unsigned(ctx, 'X', "-0#");
	combo_s(ctx);
	combo_c_p(ctx);
	g_pf_quiet = 0;
}
