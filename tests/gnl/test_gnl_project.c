#include "gnl_tester.h"
#include <ctype.h>
#ifndef _WIN32
# include <strings.h>
#endif

/*
	Subject requirements that are not about get_next_line()'s output:
		- the 3 files at the root, the header containing the prototype;
		- compiles with cc -Wall -Wextra -Werror, with and without -D BUFFER_SIZE
			(and with the values the subject asks about: 1, 9999, 10000000);
		- only read, malloc and free as external functions (no lseek, no libft);
		- no global variable; bonus: only one static variable;
		- README.md: italic first line, Description / Instructions / Resources
			sections.
	GNL_PATH (absolute path of the project) is set by the Makefile.
*/

#define GNL_OUT_SIZE 65536

typedef struct s_gnlfiles
{
	const char	*c_file;
	const char	*utils;
	const char	*header;
}	t_gnlfiles;

static const t_gnlfiles	g_mandatory = {
	"get_next_line.c", "get_next_line_utils.c", "get_next_line.h"};
static const t_gnlfiles	g_bonus = {
	"get_next_line_bonus.c", "get_next_line_utils_bonus.c",
	"get_next_line_bonus.h"};

static char	*read_file(const char *name)
{
	char	path[1024];
	FILE	*f;
	char	*buf;
	long	size;

	snprintf(path, sizeof(path), "%s/%s", GNL_PATH, name);
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

static void	check_files(t_ctx *ctx, const t_gnlfiles *files)
{
	char	label[96];
	char	*content;
	char	*squashed;
	int		i;
	int		j;

	content = read_file(files->c_file);
	snprintf(label, sizeof(label), "%s exists at the root", files->c_file);
	check_ok(ctx, label, content != NULL, "file not found");
	free(content);
	content = read_file(files->utils);
	snprintf(label, sizeof(label), "%s exists at the root", files->utils);
	check_ok(ctx, label, content != NULL, "file not found");
	free(content);
	content = read_file(files->header);
	snprintf(label, sizeof(label), "%s exists at the root", files->header);
	check_ok(ctx, label, content != NULL, "file not found");
	if (!content)
		return ;
	squashed = malloc(strlen(content) + 1);
	i = 0;
	j = 0;
	while (squashed && content[i])
	{
		if (!isspace((unsigned char)content[i]))
			squashed[j++] = content[i];
		i++;
	}
	if (squashed)
		squashed[j] = '\0';
	snprintf(label, sizeof(label), "%s declares char *get_next_line(int fd)",
		files->header);
	check_ok(ctx, label, squashed
		&& strstr(squashed, "char*get_next_line(int") != NULL,
		"prototype not found");
	free(squashed);
	free(content);
}

/* Heading line ("# ...", "## ...") containing `word`, case-insensitive. */
static int	has_heading(const char *md, const char *word)
{
	char	line[1024];
	char	*p;
	size_t	n;

	n = strlen(word);
	while (*md)
	{
		md = next_line(md, line, sizeof(line));
		if (line[0] != '#')
			continue ;
		p = line;
		while (*p)
			if (strncasecmp(p++, word, n) == 0)
				return (1);
	}
	return (0);
}

/* First line: *This project has been created as part of the 42 curriculum by <login>* */
static int	readme_first_line_ok(const char *md)
{
	static const char	*prefix
		= "This project has been created as part of the 42 curriculum by ";
	char				line[1024];
	size_t				len;
	char				mark;

	next_line(md, line, sizeof(line));
	len = strlen(line);
	while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '\t'))
		line[--len] = '\0';
	if (len < 3 || (line[0] != '*' && line[0] != '_'))
		return (0);
	mark = line[0];
	if (line[1] == mark || line[len - 1] != mark || line[len - 2] == mark)
		return (0);
	line[len - 1] = '\0';
	if (strncmp(line + 1, prefix, strlen(prefix)) != 0)
		return (0);
	return (line[1 + strlen(prefix)] != '\0'
		&& line[1 + strlen(prefix)] != ' ');
}

static void	check_readme(t_ctx *ctx)
{
	char	*md;

	md = read_file("README.md");
	check_ok(ctx, "README.md exists at the root", md != NULL, "file not found");
	if (!md)
		return ;
	check_ok(ctx, "README first line: *This project has been created as"
		" part of the 42 curriculum by <login>*", readme_first_line_ok(md),
		"must be the very first line, in italics, followed by your login(s)");
	check_ok(ctx, "README has a \"Description\" section",
		has_heading(md, "Description"),
		"no heading containing \"Description\"");
	check_ok(ctx, "README has an \"Instructions\" section",
		has_heading(md, "Instructions"),
		"no heading containing \"Instructions\"");
	check_ok(ctx, "README has a \"Resources\" section",
		has_heading(md, "Resources"),
		"no heading containing \"Resources\"");
	free(md);
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

/* First compiler line mentioning "error", else the first line. */
static void	first_error(const char *out, char *line, size_t size)
{
	const char	*p;

	p = out;
	while (*p)
	{
		p = next_line(p, line, size);
		if (strstr(line, "error"))
			return ;
	}
	next_line(out, line, size);
}

/*
	"We must be able to compile this project with and without the -D
	BUFFER_SIZE flag in addition to the usual flags." The objects of the
	BUFFER_SIZE=42 build are left in dir for the symbol checks.
*/
static int	check_compile(t_ctx *ctx, const t_gnlfiles *files, const char *dir)
{
	static const char	*flags[] = {"", " -D BUFFER_SIZE=1",
		" -D BUFFER_SIZE=9999", " -D BUFFER_SIZE=10000000",
		" -D BUFFER_SIZE=42"};
	static char			out[GNL_OUT_SIZE];
	char				cmd[2048];
	char				label[128];
	char				first[256];
	size_t				i;
	int					ok;

	ok = 0;
	i = 0;
	while (i < sizeof(flags) / sizeof(flags[0]))
	{
		snprintf(cmd, sizeof(cmd), "cd '%s' && cc -Wall -Wextra -Werror%s"
			" -c '%s/%s' '%s/%s' 2>&1", dir, flags[i], GNL_PATH,
			files->c_file, GNL_PATH, files->utils);
		ok = (run_cmd(cmd, out, sizeof(out)) == 0);
		snprintf(label, sizeof(label), "cc -Wall -Wextra -Werror%s",
			flags[i][0] ? flags[i] : " (without -D BUFFER_SIZE)");
		first_error(out, first, sizeof(first));
		check_ok(ctx, label, ok, first);
		i++;
	}
	return (ok);
}

static int	is_authorized(const char *name)
{
	static const char	*allowed[] = {"read", "malloc", "free"};
	static const char	*toolchain[] = {"__stack_chk_", "_GLOBAL_OFFSET_TABLE_",
		"dyld_stub_binder", "__chkstk_darwin"};
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

/* Symbols defined by the objects (to tell own functions from external ones). */
static int	is_defined(const char *nm_out, const char *name)
{
	char		line[512];
	char		*tok[3];
	int			n;

	while (*nm_out)
	{
		nm_out = next_line(nm_out, line, sizeof(line));
		n = 0;
		tok[0] = strtok(line, " \t");
		while (tok[n] && ++n < 3)
			tok[n] = strtok(NULL, " \t");
		if (n == 3 && strcmp(tok[2], name) == 0 && strcmp(tok[1], "U") != 0)
			return (1);
	}
	return (0);
}

/* Compiler-generated local labels, not variables. */
static int	is_internal(const char *name)
{
	return (name[0] == '.' || name[0] == 'L' || strncmp(name, "ltmp", 4) == 0
		|| strncmp(name, "l_", 2) == 0);
}

/*
	nm output: "addr type name" (defined) or "type name" (undefined).
	U: external function; B D C G S R: global variable;
	b d s r: static variable (local or file-scope).
*/
static void	check_symbols(t_ctx *ctx, const char *dir, int max_statics)
{
	static char	out[GNL_OUT_SIZE];
	char		cmd[1200];
	char		line[512];
	char		label[160];
	char		*tok[3];
	const char	*p;
	const char	*name;
	int			n;
	int			bad;
	int			statics;

	snprintf(cmd, sizeof(cmd), "nm '%s'/*.o 2>/dev/null", dir);
	if (run_cmd(cmd, out, sizeof(out)) != 0 || !out[0])
	{
		if (ctx->verbose)
			printf("    " YELLOW "-" RESET " symbols not checked"
				" (nm unavailable)\n");
		return ;
	}
	bad = 0;
	statics = 0;
	p = out;
	while (*p)
	{
		p = next_line(p, line, sizeof(line));
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
		if (strcmp(tok[n - 2], "U") == 0 && !is_authorized(name)
			&& !is_defined(out, tok[n - 1]))
		{
			snprintf(label, sizeof(label), "forbidden external function: %s",
				name);
			check_ok(ctx, label, 0, "authorized: read, malloc, free"
				" (lseek and libft are forbidden)");
			bad++;
		}
		else if (n == 3 && strchr("BDCGSR", tok[1][0]) && tok[1][1] == '\0')
		{
			snprintf(label, sizeof(label), "global variable (forbidden): %s",
				name);
			check_ok(ctx, label, 0, NULL);
			bad++;
		}
		else if (n == 3 && strchr("bdsr", tok[1][0]) && tok[1][1] == '\0'
			&& !is_internal(tok[2]))
			statics++;
	}
	if (!bad)
		check_ok(ctx, "only read, malloc, free; no global variable", 1, NULL);
	if (max_statics > 0)
	{
		snprintf(label, sizeof(label), "only one static variable (found %d)",
			statics);
		check_ok(ctx, label, statics <= max_statics, NULL);
	}
}

static void	check_build(t_ctx *ctx, const t_gnlfiles *files, int max_statics)
{
	char	dir[] = "/tmp/gnl_tester_XXXXXX";
	char	cmd[64];

	if (!mkdtemp(dir))
		return ;
	if (check_compile(ctx, files, dir))
		check_symbols(ctx, dir, max_statics);
	snprintf(cmd, sizeof(cmd), "rm -rf '%s'", dir);
	if (system(cmd) != 0)
		return ;
}

#endif

void	test_gnl_project(t_ctx *ctx)
{
	check_files(ctx, &g_mandatory);
#ifndef _WIN32
	check_build(ctx, &g_mandatory, 0);
#endif
	check_readme(ctx);
}

void	test_gnl_bonus_project(t_ctx *ctx)
{
	check_files(ctx, &g_bonus);
#ifndef _WIN32
	check_build(ctx, &g_bonus, 1);
#endif
}
