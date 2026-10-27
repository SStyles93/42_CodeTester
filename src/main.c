#include "tester.h"
#ifndef _WIN32
# include <strings.h>
#endif

#ifdef _WIN32
# include <windows.h>
static void	win_init_console(void)
{
	HANDLE	h;
	DWORD	mode;

	h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleMode(h, &mode))
		SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	SetConsoleOutputCP(65001);
}
#endif

static void	run_all_projects(t_project **projects, int count, int verbose)
{
	int	i;

	i = 0;
	while (i < count)
		run_project_all(projects[i++], verbose);
}

/*
	Project names are case-insensitive. "printf" also matches every category
	of that project ("Printf_mandatory", "Printf_bonus").
*/
static int	project_matches(const char *project, const char *name)
{
	size_t	n;

	if (strcasecmp(project, name) == 0)
		return (1);
	n = strlen(name);
	return (strncasecmp(project, name, n) == 0 && project[n] == '_');
}

/*
	argv argument other than "all": a project name (e.g. "libft", "printf",
	"printf_bonus") runs that whole project, otherwise it is looked up as a
	suite name in every project.
*/
static int	run_by_name(t_project **projects, int count, const char *name,
	int verbose)
{
	int	i;
	int	j;
	int	found;

	found = 0;
	i = 0;
	while (i < count)
	{
		if (project_matches(projects[i]->name, name))
		{
			run_project_all(projects[i], verbose);
			found = 1;
		}
		i++;
	}
	if (found)
		return (0);
	i = 0;
	while (i < count)
	{
		j = 0;
		while (j < projects[i]->suite_count)
		{
			if (strcmp(projects[i]->suites[j].name, name) == 0)
			{
				run_suite(projects[i], &projects[i]->suites[j], verbose);
				return (0);
			}
			j++;
		}
		i++;
	}
	fprintf(stderr, RED "Error:" RESET " no project or suite named '%s'\n",
		name);
	return (1);
}

int	main(int argc, char **argv)
{
	t_project	**projects;
	int			project_count;
	int			verbose;
	int			i;

#ifdef _WIN32
	win_init_console();
#endif
	projects = get_all_projects(&project_count);
	verbose = 0;
	i = 1;
	while (i < argc && argv[i][0] == '-')
	{
		if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
			verbose = 1;
		else if ((strcmp(argv[i], "-t") == 0
				|| strcmp(argv[i], "--timeout") == 0) && i + 1 < argc)
			g_test_timeout = atoi(argv[++i]);
		i++;
	}
	if (project_count == 0)
	{
		fprintf(stderr, RED "Error:" RESET " no project detected (expected"
			" a sibling directory such as ../libft or ../printf)\n");
		return (1);
	}
	if (i >= argc)
	{
		if (isatty(STDIN_FILENO))
			launch_menu(projects, project_count);
		else
			run_all_projects(projects, project_count, verbose);
	}
	else if (strcmp(argv[i], "all") == 0)
		run_all_projects(projects, project_count, verbose);
	else
		return (run_by_name(projects, project_count, argv[i], verbose));
	return (0);
}
