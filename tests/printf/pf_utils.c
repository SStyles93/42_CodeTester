#include "pf_tester.h"

#define PF_SHOW_MAX 120

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

void	pf_print_escaped(const char *s, int len)
{
	int				i;
	unsigned char	c;

	putchar('"');
	i = 0;
	while (i < len && i < PF_SHOW_MAX)
	{
		c = (unsigned char)s[i];
		if (c == '\n')
			fputs("\\n", stdout);
		else if (c == '\t')
			fputs("\\t", stdout);
		else if (c == '"' || c == '\\')
			printf("\\%c", c);
		else if (c < 32 || c >= 127)
			printf("\\x%02x", c);
		else
			putchar(c);
		i++;
	}
	putchar('"');
	if (len > PF_SHOW_MAX)
		printf("... (%d bytes)", len);
}

static int	first_diff(const char *a, int alen, const char *b, int blen)
{
	int	i;

	i = 0;
	while (i < alen && i < blen && a[i] == b[i])
		i++;
	return (i);
}

void	pf_report(t_ctx *ctx, const char *label, const t_pfcase *c)
{
	int	exp_len;
	int	ok;

	exp_len = c->exp_ret;
	if (exp_len >= PF_BUF_SIZE)
		exp_len = PF_BUF_SIZE - 1;
	if (!c->cap_ok)
		ok = 0;
	else if (c->exp_ret < 0)
		ok = (c->got_ret == c->exp_ret);
	else
		ok = (c->got_ret == c->exp_ret && c->got_len == exp_len
				&& memcmp(g_pf_exp, g_pf_got, exp_len) == 0);
	if (ok)
		ctx->pass++;
	else
		ctx->fail++;
	if (!ctx->verbose)
		return ;
	if (ok)
	{
		printf("    " GREEN "✓" RESET " %s → ", label);
		pf_print_escaped(g_pf_got, c->got_len);
		printf(" (%d)\n", c->got_ret);
		return ;
	}
	printf("    " RED "✗" RESET " %s\n", label);
	if (!c->cap_ok)
	{
		printf("        could not capture stdout\n");
		return ;
	}
	printf("        expected: ");
	if (c->exp_ret < 0)
		printf("(error)");
	else
		pf_print_escaped(g_pf_exp, exp_len);
	printf(" return %d\n        got:      ", c->exp_ret);
	pf_print_escaped(g_pf_got, c->got_len);
	printf(" return %d\n", c->got_ret);
	if (c->exp_ret >= 0)
		printf(DIM "        first difference at byte %d" RESET "\n",
			first_diff(g_pf_exp, exp_len, g_pf_got, c->got_len));
}
