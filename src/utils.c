#include "tester.h"

/*
	Every result is printed on one line:
		✓ <name> → <value>
		✗ <name> → expected: <value> → got: <value>
	Strings are quoted and escaped (\n, \t, \xNN), and cut after SHOW_MAX bytes.
*/

#define SHOW_MAX 120

const char	*fmt_bytes(char *buf, size_t size, const char *s, size_t len)
{
	size_t			i;
	size_t			j;
	unsigned char	c;

	if (!s)
	{
		snprintf(buf, size, "NULL");
		return (buf);
	}
	j = 0;
	buf[j++] = '"';
	i = 0;
	while (i < len && i < SHOW_MAX && j + 8 < size)
	{
		c = (unsigned char)s[i++];
		if (c == '\n')
			j += snprintf(buf + j, size - j, "\\n");
		else if (c == '\t')
			j += snprintf(buf + j, size - j, "\\t");
		else if (c == '"' || c == '\\')
			j += snprintf(buf + j, size - j, "\\%c", c);
		else if (c < 32 || c >= 127)
			j += snprintf(buf + j, size - j, "\\x%02x", c);
		else
			buf[j++] = c;
	}
	buf[j++] = '"';
	buf[j] = '\0';
	if (len > SHOW_MAX)
		snprintf(buf + j, size - j, "... (%zu bytes)", len);
	return (buf);
}

const char	*fmt_str(char *buf, size_t size, const char *s)
{
	return (fmt_bytes(buf, size, s, s ? strlen(s) : 0));
}

void	report_pass(t_ctx *ctx, const char *label, const char *value)
{
	ctx->pass++;
	if (!ctx->verbose)
		return ;
	if (value)
		printf("    " GREEN "✓" RESET " %s → %s\n", label, value);
	else
		printf("    " GREEN "✓" RESET " %s\n", label);
}

void	report_fail(t_ctx *ctx, const char *label, const char *expected,
	const char *got)
{
	ctx->fail++;
	if (ctx->verbose)
		printf("    " RED "✗" RESET " %s → expected: %s → got: %s\n",
			label, expected, got);
}

/* Yes/no requirement: "✗ <name> → <why>" when it is not met. */
void	check_ok(t_ctx *ctx, const char *label, int ok, const char *why)
{
	if (ok)
	{
		report_pass(ctx, label, NULL);
		return ;
	}
	ctx->fail++;
	if (!ctx->verbose)
		return ;
	if (why && why[0])
		printf("    " RED "✗" RESET " %s → %s\n", label, why);
	else
		printf("    " RED "✗" RESET " %s\n", label);
}

void	check_bool(t_ctx *ctx, const char *label, int expected, int got)
{
	if (!expected == !got)
		report_pass(ctx, label, got ? "true" : "false");
	else
		report_fail(ctx, label, expected ? "true" : "false",
			got ? "true" : "false");
}

void	check_int(t_ctx *ctx, const char *label, int expected, int got)
{
	char	e[32];
	char	g[32];

	snprintf(e, sizeof(e), "%d", expected);
	snprintf(g, sizeof(g), "%d", got);
	if (expected == got)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, e, g);
}

void	check_uint(t_ctx *ctx, const char *label, unsigned int expected,
	unsigned int got)
{
	char	e[32];
	char	g[32];

	snprintf(e, sizeof(e), "%u", expected);
	snprintf(g, sizeof(g), "%u", got);
	if (expected == got)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, e, g);
}

void	check_size(t_ctx *ctx, const char *label, size_t expected, size_t got)
{
	char	e[32];
	char	g[32];

	snprintf(e, sizeof(e), "%zu", expected);
	snprintf(g, sizeof(g), "%zu", got);
	if (expected == got)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, e, g);
}

static const char	*fmt_char(char *buf, size_t size, char c)
{
	char	esc[16];

	fmt_bytes(esc, sizeof(esc), &c, 1);
	esc[0] = '\'';
	esc[strlen(esc) - 1] = '\'';
	snprintf(buf, size, "%s (%d)", esc, (int)(unsigned char)c);
	return (buf);
}

void	check_char(t_ctx *ctx, const char *label, char expected, char got)
{
	char	e[32];
	char	g[32];

	fmt_char(e, sizeof(e), expected);
	fmt_char(g, sizeof(g), got);
	if (expected == got)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, e, g);
}

void	check_str(t_ctx *ctx, const char *label, const char *expected,
	const char *got)
{
	char	e[SHOW_BUF];
	char	g[SHOW_BUF];
	int		eq;

	if (expected == NULL || got == NULL)
		eq = (expected == got);
	else
		eq = (strcmp(expected, got) == 0);
	fmt_str(g, sizeof(g), got);
	if (eq)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, fmt_str(e, sizeof(e), expected), g);
}

void	check_mem(t_ctx *ctx, const char *label, const void *expected,
	const void *got, size_t n)
{
	const unsigned char	*e;
	const unsigned char	*g;
	char				ebuf[48];
	char				gbuf[48];
	size_t				i;

	if (memcmp(expected, got, n) == 0)
	{
		snprintf(gbuf, sizeof(gbuf), "%zu bytes identical", n);
		report_pass(ctx, label, gbuf);
		return ;
	}
	e = (const unsigned char *)expected;
	g = (const unsigned char *)got;
	i = 0;
	while (i < n && e[i] == g[i])
		i++;
	snprintf(ebuf, sizeof(ebuf), "byte %zu = 0x%02x", i, e[i]);
	snprintf(gbuf, sizeof(gbuf), "byte %zu = 0x%02x", i, g[i]);
	report_fail(ctx, label, ebuf, gbuf);
}

static const char	*fmt_ptr(char *buf, size_t size, const void *p)
{
	if (p == NULL)
		snprintf(buf, size, "NULL");
	else
		snprintf(buf, size, "%p", p);
	return (buf);
}

void	check_ptr(t_ctx *ctx, const char *label, const void *expected,
	const void *got)
{
	char	e[32];
	char	g[32];

	fmt_ptr(g, sizeof(g), got);
	if (expected == got)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, fmt_ptr(e, sizeof(e), expected), g);
}

void	check_not_null(t_ctx *ctx, const char *label, const void *got)
{
	if (got != NULL)
		report_pass(ctx, label, "non-NULL");
	else
		report_fail(ctx, label, "non-NULL", "NULL");
}

/* Pointers into the same buffer: compared as offsets from base, or NULL. */
void	check_offset(t_ctx *ctx, const char *label, const void *base,
	const void *expected, const void *got)
{
	char	e[48];
	char	g[48];

	if (expected)
		snprintf(e, sizeof(e), "offset %ld",
			(long)((const char *)expected - (const char *)base));
	else
		snprintf(e, sizeof(e), "NULL");
	if (got)
		snprintf(g, sizeof(g), "offset %ld",
			(long)((const char *)got - (const char *)base));
	else
		snprintf(g, sizeof(g), "NULL");
	if (expected == got)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, e, g);
}

/* memcmp/strncmp-like results: only the sign matters. */
void	check_sign(t_ctx *ctx, const char *label, int expected, int got)
{
	static const char	*names[] = {"< 0", "0", "> 0"};
	char				g[48];
	int					esign;
	int					gsign;

	esign = (expected > 0) - (expected < 0);
	gsign = (got > 0) - (got < 0);
	if (gsign)
		snprintf(g, sizeof(g), "%s (%d)", names[gsign + 1], got);
	else
		snprintf(g, sizeof(g), "0");
	if (esign == gsign)
		report_pass(ctx, label, g);
	else
		report_fail(ctx, label, names[esign + 1], g);
}
