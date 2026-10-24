#include "pf_tester.h"
#include <ctype.h>
#ifndef _WIN32
# include <dirent.h>
# include <strings.h>
#endif

/*
** Subject requirements that are not about ft_printf's output:
**   - libftprintf.a at the root, built with ar (libtool forbidden);
**   - a header (.h at the root or one directory below) containing the
**     prototype;
**   - Makefile with NAME, all, clean, fclean, re (+ bonus), no relink;
**   - only malloc, free, write (and va_*) as external functions.
** PRINTF_PATH (absolute path of the project) is set by the Makefile.
*/

#ifndef PRINTF_PATH
# define PRINTF_PATH "../printf"
#endif

#define PF_OUT_SIZE 65536
#define PF_MAX_SYMS 1024

typedef struct s_syms
{
	char	names[PF_MAX_SYMS][64];
	int		count;
}	t_syms;

static char	*read_file(const char *name)
{
	char	path[1024];
	FILE	*f;
	char	*buf;
	long	size;

	snprintf(path, sizeof(path), "%s/%s", PRINTF_PATH, name);
	f = fopen(path, "rb");
	if (!f)
		return (NULL);
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	rewind(f);
	buf = malloc(size + 1);
	if (buf)
		buf[fread(buf, 1, size, f)] = '\0';
	fclose(f);
	return (buf);
}

/* Copy line starting at s into line (without '\n'), return the next line. */
static const char	*next_line(const char *s, char *line, size_t size)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '\n')
	{
		if (i + 1 < size)
			line[i] = s[i];
		i++;
	}
	line[i + 1 < size ? i : size - 1] = '\0';
	if (i > 0 && line[strlen(line) - 1] == '\r')
		line[strlen(line) - 1] = '\0';
	return (s[i] ? s + i + 1 : s + i);
}

/* "target:" rule (possibly with other targets on the same line). */
static int	mk_has_target(const char *mk, const char *target)
{
	char		line[1024];
	char		*colon;
	char		*tok;

	while (*mk)
	{
		mk = next_line(mk, line, sizeof(line));
		colon = strchr(line, ':');
		if (line[0] == '\t' || line[0] == '#' || !colon || colon[1] == '=')
			continue ;
		*colon = '\0';
		tok = strtok(line, " \t");
		while (tok)
		{
			if (strcmp(tok, target) == 0)
				return (1);
			tok = strtok(NULL, " \t");
		}
	}
	return (0);
}

/* "VAR =", "VAR :=", "VAR ?=" ... at the start of a line. */
static int	mk_has_var(const char *mk, const char *var)
{
	char	line[1024];
	char	*p;
	size_t	n;

	n = strlen(var);
	while (*mk)
	{
		mk = next_line(mk, line, sizeof(line));
		if (strncmp(line, var, n) != 0)
			continue ;
		p = line + n;
		while (*p == ' ' || *p == '\t')
			p++;
		if (*p == '=' || strncmp(p, ":=", 2) == 0 || strncmp(p, "?=", 2) == 0
			|| strncmp(p, "::=", 3) == 0)
			return (1);
	}
	return (0);
}

/* name (relative to PRINTF_PATH) is a .h declaring ft_printf. */
static int	declares_ft_printf(const char *name)
{
	char	*h;
	int		i;
	int		j;
	int		found;

	h = read_file(name);
	if (!h)
		return (0);
	i = 0;
	j = 0;
	while (h[i])
	{
		if (!isspace((unsigned char)h[i]))
			h[j++] = h[i];
		i++;
	}
	h[j] = '\0';
	found = (strstr(h, "intft_printf(constchar*") != NULL);
	free(h);
	return (found);
}

#ifndef _WIN32

/* Every .h in dir (relative to PRINTF_PATH, "" = root) and its subdirs. */
static void	scan_headers(const char *dir, int depth, int *headers, int *found)
{
	char			path[1024];
	char			name[1024];
	DIR				*d;
	struct dirent	*e;
	size_t			n;

	snprintf(path, sizeof(path), "%s/%s", PRINTF_PATH, dir);
	d = opendir(path);
	while (d && (e = readdir(d)) != NULL)
	{
		if (e->d_name[0] == '.')
			continue ;
		snprintf(name, sizeof(name), "%s%s%s", dir, dir[0] ? "/" : "",
			e->d_name);
		n = strlen(e->d_name);
		if (n > 2 && strcmp(e->d_name + n - 2, ".h") == 0)
		{
			(*headers)++;
			*found |= declares_ft_printf(name);
		}
		else if (depth == 0)
			scan_headers(name, 1, headers, found);
	}
	if (d)
		closedir(d);
}

#endif

static void	check_files(t_ctx *ctx)
{
	char	path[1024];
	int		headers;
	int		found;

	snprintf(path, sizeof(path), "%s/libftprintf.a", PRINTF_PATH);
	check_ok(ctx, "libftprintf.a exists at the root", access(path, F_OK) == 0,
		path);
	headers = 0;
	found = 0;
#ifndef _WIN32
	scan_headers("", 0, &headers, &found);
#else
	headers = 1;
	found = declares_ft_printf("ft_printf.h");
#endif
	check_ok(ctx, "a header declares int ft_printf(const char *, ...)", found,
		headers ? "prototype not found in any .h" : "no .h file found");
}

static void	check_makefile(t_ctx *ctx)
{
	static const char	*rules[] = {"all", "clean", "fclean", "re"};
	char				label[64];
	char				*mk;
	size_t				i;

	mk = read_file("Makefile");
	check_ok(ctx, "Makefile exists at the root", mk != NULL, "file not found");
	if (!mk)
		return ;
	check_ok(ctx, "Makefile defines NAME", mk_has_var(mk, "NAME"),
		"no \"NAME =\" line");
	check_ok(ctx, "Makefile names the library libftprintf.a",
		strstr(mk, "libftprintf.a") != NULL, "\"libftprintf.a\" not found");
	i = 0;
	while (i < sizeof(rules) / sizeof(rules[0]))
	{
		snprintf(label, sizeof(label), "Makefile has a '%s' rule", rules[i]);
		check_ok(ctx, label, mk_has_target(mk, rules[i]), "rule not found");
		i++;
	}
	check_ok(ctx, "Makefile uses ar", strstr(mk, "ar ") != NULL
		|| strstr(mk, "ar\t") != NULL || strstr(mk, "$(AR)") != NULL,
		"neither \"ar\" nor $(AR) found");
	check_ok(ctx, "Makefile does not use libtool (forbidden)",
		strstr(mk, "libtool") == NULL, "libtool found");
	check_ok(ctx, "Makefile uses -Wall -Wextra -Werror",
		strstr(mk, "-Wall") && strstr(mk, "-Wextra") && strstr(mk, "-Werror"),
		"at least one of the flags is missing");
	free(mk);
}

#ifndef _WIN32

static int	run_cmd(const char *cmd, char *out, size_t size)
{
	FILE	*p;
	size_t	n;

	out[0] = '\0';
	fflush(stdout);
	p = popen(cmd, "r");
	if (!p)
		return (-1);
	n = fread(out, 1, size - 1, p);
	out[n] = '\0';
	return (pclose(p));
}

/* A line of make output that compiles, archives or links something. */
static const char	*find_build_line(const char *out, char *line, size_t size)
{
	while (*out)
	{
		out = next_line(out, line, size);
		if (strncmp(line, "make", 4) == 0)
			continue ;
		if (strstr(line, " -c ") || strncmp(line, "ar ", 3) == 0
			|| strstr(line, " ar ") || strstr(line, "ranlib")
			|| strstr(line, " -o "))
			return (line);
	}
	return (NULL);
}

static void	check_relink(t_ctx *ctx, const char *rule)
{
	static char	out[PF_OUT_SIZE];
	char		cmd[1200];
	char		label[64];
	char		line[512];
	const char	*bad;

	snprintf(cmd, sizeof(cmd), "make -C '%s' %s 2>&1", PRINTF_PATH, rule);
	run_cmd(cmd, out, sizeof(out));
	snprintf(label, sizeof(label), "'make%s%s' twice does not relink",
		rule[0] ? " " : "", rule);
	if (run_cmd(cmd, out, sizeof(out)) != 0)
	{
		check_ok(ctx, label, 0, "make failed");
		return ;
	}
	bad = find_build_line(out, line, sizeof(line));
	check_ok(ctx, label, bad == NULL, bad);
}

static int	sym_has(const t_syms *s, const char *name)
{
	int	i;

	i = 0;
	while (i < s->count)
		if (strcmp(s->names[i++], name) == 0)
			return (1);
	return (0);
}

static void	sym_add(t_syms *s, const char *name)
{
	if (s->count < PF_MAX_SYMS && !sym_has(s, name))
		snprintf(s->names[s->count++], sizeof(s->names[0]), "%s", name);
}

static int	is_authorized(const char *name)
{
	static const char	*allowed[] = {"malloc", "free", "write"};
	static const char	*toolchain[] = {"__stack_chk_", "_GLOBAL_OFFSET_TABLE_",
		"__asan_", "__ubsan_", "__sanitizer_", "__lsan_", "dyld_stub_binder",
		"__chkstk_darwin"};
	size_t				i;

	i = 0;
	while (i < sizeof(allowed) / sizeof(allowed[0]))
		if (strcmp(name, allowed[i++]) == 0)
			return (1);
	i = 0;
	while (i < sizeof(toolchain) / sizeof(toolchain[0]))
	{
		if (strncmp(name, toolchain[i], strlen(toolchain[i])) == 0)
			return (1);
		i++;
	}
	return (0);
}

static void	parse_nm(const char *out, t_syms *def, t_syms *undef)
{
	char		line[512];
	char		*tok[3];
	const char	*name;
	int			n;

	while (*out)
	{
		out = next_line(out, line, sizeof(line));
		n = 0;
		tok[0] = strtok(line, " \t");
		while (tok[n] && ++n < 3)
			tok[n] = strtok(NULL, " \t");
		if (n < 2 || tok[n - 1][strlen(tok[n - 1]) - 1] == ':')
			continue ;
		name = tok[n - 1];
#ifdef __APPLE__
		if (name[0] == '_')
			name++;
#endif
		if (strcmp(tok[n - 2], "U") == 0)
			sym_add(undef, name);
		else if (strcmp(tok[n - 2], "w") != 0 && strcmp(tok[n - 2], "v") != 0)
			sym_add(def, name);
	}
}

static void	check_external_functions(t_ctx *ctx)
{
	static char		out[PF_OUT_SIZE * 4];
	static t_syms	def;
	static t_syms	undef;
	char			cmd[1200];
	char			label[128];
	int				forbidden;
	int				i;

	snprintf(cmd, sizeof(cmd), "nm -g '%s/libftprintf.a' 2>/dev/null",
		PRINTF_PATH);
	if (run_cmd(cmd, out, sizeof(out)) != 0 || !out[0])
	{
		if (ctx->verbose)
			printf("    " YELLOW "-" RESET " external functions not checked"
				" (nm unavailable)\n");
		return ;
	}
	def.count = 0;
	undef.count = 0;
	parse_nm(out, &def, &undef);
	forbidden = 0;
	i = 0;
	while (i < undef.count)
	{
		if (!sym_has(&def, undef.names[i]) && !is_authorized(undef.names[i]))
		{
			snprintf(label, sizeof(label), "forbidden external function: %s",
				undef.names[i]);
			check_ok(ctx, label, 0, "authorized: malloc, free, write, va_*");
			forbidden++;
		}
		i++;
	}
	if (!forbidden)
		check_ok(ctx, "only authorized external functions"
			" (malloc, free, write, va_*)", 1, NULL);
}

#endif

void	test_pf_project(t_ctx *ctx)
{
	check_files(ctx);
	check_makefile(ctx);
#ifndef _WIN32
	check_relink(ctx, "");
	check_relink(ctx, "all");
	check_external_functions(ctx);
#endif
}

void	test_pf_bonus_project(t_ctx *ctx)
{
	char	*mk;

	mk = read_file("Makefile");
	check_ok(ctx, "Makefile has a 'bonus' rule (42 rules)",
		mk && mk_has_target(mk, "bonus"), "rule not found");
	free(mk);
#ifndef _WIN32
	check_relink(ctx, "bonus");
#endif
}
