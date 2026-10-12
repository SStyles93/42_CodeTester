#include "tester.h"

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
	if (i >= argc)
	{
		if (isatty(STDIN_FILENO))
			launch_menu(projects, project_count);
		else
			run_project_all(projects[0], verbose);
	}
	else if (strcmp(argv[i], "all") == 0)
	{
		run_project_all(projects[0], verbose);
	}
	else
	{
		run_suite_by_name(projects[0], argv[i], verbose);
	}
	return (0);
}
