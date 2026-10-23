#include "pf_tester.h"
#include <string.h>

char	g_pf_exp[PF_BUF_SIZE];
char	g_pf_got[PF_BUF_SIZE];

int	pf_capture_begin(t_pfcap *cap)
{
	fflush(stdout);
	cap->saved_fd = -1;
	cap->file = tmpfile();
	if (!cap->file)
		return (-1);
	cap->saved_fd = dup(STDOUT_FILENO);
	if (cap->saved_fd < 0 || dup2(fileno(cap->file), STDOUT_FILENO) < 0)
	{
		if (cap->saved_fd >= 0)
			close(cap->saved_fd);
		cap->saved_fd = -1;
		fclose(cap->file);
		cap->file = NULL;
		return (-1);
	}
	return (0);
}

int	pf_capture_end(t_pfcap *cap, char *buf, size_t size)
{
	size_t	n;

	buf[0] = '\0';
	if (!cap->file)
		return (0);
	dup2(cap->saved_fd, STDOUT_FILENO);
	close(cap->saved_fd);
	rewind(cap->file);
	n = fread(buf, 1, size - 1, cap->file);
	buf[n] = '\0';
	fclose(cap->file);
	cap->file = NULL;
	return ((int)n);
}

/* Same length as fmt_bytes() shows: past it, say where the outputs differ. */
#define PF_SHOW_MAX 120

static int	first_diff(const char *a, int alen, const char *b, int blen)
{
	int	i;

	i = 0;
	while (i < alen && i < blen && a[i] == b[i])
		i++;
	return (i);
}

/* "<escaped output> (return n)", or "error (return n)" for a failed call. */
static const char	*fmt_output(char *buf, size_t size, const char *s, int len,
	int ret)
{
	char	esc[SHOW_BUF];

	if (ret < 0 && len <= 0)
		snprintf(buf, size, "error (return %d)", ret);
	else
		snprintf(buf, size, "%s (return %d)",
			fmt_bytes(esc, sizeof(esc), s, len < 0 ? 0 : len), ret);
	return (buf);
}

/*
** Compares ft_printf's output and return value to snprintf's.
** alt_exp (may be NULL) is a second accepted output, for combos where two
** platforms produce different but equally correct text (e.g. "%p" with
** NULL: "(nil)" vs "0x0"). The host snprintf result is always the primary
** expected; a "[platform alt]" tag is shown when alt_exp matched.
*/
void	pf_report_alt(t_ctx *ctx, const char *label,
		const t_pfcase *c, const char *alt_exp)
{
	char	exp[SHOW_BUF * 2 + 64];
	char	got[SHOW_BUF + 64];
	char	alt[SHOW_BUF];
	int		exp_len;
	int		alt_len;
	int		ok;
	int		ok_alt;
	size_t	n;

	if (!c->cap_ok)
	{
		check_ok(ctx, label, 0, "could not capture stdout");
		return ;
	}
	exp_len = c->exp_ret;
	if (exp_len >= PF_BUF_SIZE)
		exp_len = PF_BUF_SIZE - 1;
	alt_len = alt_exp ? (int)strlen(alt_exp) : -1;
	if (c->exp_ret < 0)
		ok = (c->got_ret == c->exp_ret);
	else
		ok = (c->got_ret == c->exp_ret && c->got_len == exp_len
				&& memcmp(g_pf_exp, g_pf_got, exp_len) == 0);
	ok_alt = (!ok && alt_exp && c->got_ret == alt_len && c->got_len == alt_len
			&& memcmp(alt_exp, g_pf_got, alt_len) == 0);
	fmt_output(got, sizeof(got), g_pf_got, c->got_len, c->got_ret);
	if (ok || ok_alt)
	{
		if (ok_alt)
			strncat(got, " " DIM "[platform alt]" RESET,
				sizeof(got) - strlen(got) - 1);
		report_pass(ctx, label, got);
		return ;
	}
	fmt_output(exp, sizeof(exp), g_pf_exp, c->exp_ret < 0 ? 0 : exp_len,
		c->exp_ret);
	if (alt_exp && c->exp_ret >= 0)
	{
		n = strlen(exp);
		snprintf(exp + n, sizeof(exp) - n, " or %s (return %d)",
			fmt_bytes(alt, sizeof(alt), alt_exp, alt_len), alt_len);
	}
	if (c->exp_ret >= 0 && (exp_len > PF_SHOW_MAX || c->got_len > PF_SHOW_MAX))
	{
		n = strlen(got);
		snprintf(got + n, sizeof(got) - n, " (first difference at byte %d)",
			first_diff(g_pf_exp, exp_len, g_pf_got, c->got_len));
	}
	report_fail(ctx, label, exp, got);
}

void	pf_report(t_ctx *ctx, const char *label, const t_pfcase *c)
{
	pf_report_alt(ctx, label, c, NULL);
}
