#include "tester.h"

#define KEY_UP      'u'
#define KEY_DOWN    'd'
#define KEY_ENTER   '\n'
#define KEY_QUIT    'q'
#define KEY_BACK    'b'
#define KEY_VERBOSE 'v'

/* ── Platform-specific raw-mode / key-reading ─────────────────────────────── */

#ifdef _WIN32

# include <conio.h>
# include <windows.h>

static DWORD	g_orig_mode;
static HANDLE	g_hout;

static void	restore_terminal(void)
{
	SetConsoleMode(g_hout, g_orig_mode);
	printf("\033[?25h");
	fflush(stdout);
}

static void	enable_raw(void)
{
	static int	initialized = 0;
	DWORD		mode;

	if (!initialized)
	{
		g_hout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(g_hout, &g_orig_mode);
		/* Enable VT100 sequences + UTF-8 output */
		mode = g_orig_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
			| DISABLE_NEWLINE_AUTO_RETURN;
		SetConsoleMode(g_hout, mode);
		SetConsoleOutputCP(65001);
		atexit(restore_terminal);
		initialized = 1;
	}
	printf("\033[?25l");
	fflush(stdout);
}

static int	read_key(void)
{
	int	c;

	c = _getch();
	if (c == 'q' || c == 'Q')
		return (KEY_QUIT);
	if (c == 'b' || c == 'B')
		return (KEY_BACK);
	if (c == 'v' || c == 'V')
		return (KEY_VERBOSE);
	if (c == '\r' || c == '\n')
		return (KEY_ENTER);
	/* Arrow keys send 0xE0 (or 0x00 for numpad) then a scan code */
	if (c == 0 || c == 0xE0)
	{
		c = _getch();
		if (c == 0x48)
			return (KEY_UP);
		if (c == 0x50)
			return (KEY_DOWN);
		return (0);
	}
	return (c);
}

#else  /* ── POSIX / Linux / macOS ─────────────────────────────────────────── */

static struct termios	g_orig;

static void	restore_terminal(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_orig);
	printf("\033[?25h");
}

static void	enable_raw(void)
{
	static int		initialized = 0;
	struct termios	raw;

	if (!initialized)
	{
		tcgetattr(STDIN_FILENO, &g_orig);
		atexit(restore_terminal);
		initialized = 1;
	}
	raw = g_orig;
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	printf("\033[?25l");
}

static int	read_key(void)
{
	unsigned char	c;
	unsigned char	seq[3];

	if (read(STDIN_FILENO, &c, 1) != 1)
		return (0);
	if (c == 'q' || c == 'Q')
		return (KEY_QUIT);
	if (c == 'b' || c == 'B')
		return (KEY_BACK);
	if (c == 'v' || c == 'V')
		return (KEY_VERBOSE);
	if (c == '\n' || c == '\r')
		return (KEY_ENTER);
	if (c == 27)
	{
		if (read(STDIN_FILENO, &seq[0], 1) != 1)
			return (27);
		if (read(STDIN_FILENO, &seq[1], 1) != 1)
			return (27);
		if (seq[0] == '[' && seq[1] == 'A')
			return (KEY_UP);
		if (seq[0] == '[' && seq[1] == 'B')
			return (KEY_DOWN);
	}
	return ((int)c);
}

#endif /* _WIN32 */

/* ── Drawing / menu logic (shared) ─────────────────────────────────────────── */

static void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

static void	draw_header(const char *subtitle)
{
	clear();
	printf("\033[2J\033[H");
	printf(BOLD CYAN "┌──────────────────────────────────────────┐\n" RESET);
	printf(BOLD CYAN "│" RESET "  " BOLD "42 School C Tester" RESET
		"  " DIM "%-20s" RESET BOLD CYAN "│\n" RESET, subtitle);
	printf(BOLD CYAN "└──────────────────────────────────────────┘\n\n" RESET);
}

static int	project_menu(t_project **projects, int count)
{
	int	cursor;
	int	key;
	int	i;

	cursor = 0;
	while (1)
	{
		draw_header("Select project");
		i = 0;
		while (i < count)
		{
			if (i == cursor)
				printf("  " BOLD CYAN "▶ %s" RESET "\n", projects[i]->name);
			else
				printf("    %s\n", projects[i]->name);
			i++;
		}
		printf("\n  " DIM "↑/↓ Navigate   Enter Select   q Quit" RESET "\n");
		key = read_key();
		if (key == KEY_QUIT)
			return (-1);
		if (key == KEY_UP && cursor > 0)
			cursor--;
		if (key == KEY_DOWN && cursor < count - 1)
			cursor++;
		if (key == KEY_ENTER)
			return (cursor);
	}
}

/* Returns suite index, -1 for Run All, -2 for Back, -3 for Quit */
static int	suite_menu(t_project *project, int *verbose)
{
	int	cursor;
	int	key;
	int	total;
	int	i;

	cursor = 0;
	total = project->suite_count + 1;
	while (1)
	{
		draw_header(project->name);
		printf("  " DIM "[v] verbose: " RESET "%s\n\n",
			*verbose ? GREEN "ON " RESET : "off");
		if (cursor == 0)
			printf("  " BOLD CYAN "▶ [Run All]" RESET "\n");
		else
			printf("    [Run All]\n");
		i = 0;
		while (i < project->suite_count)
		{
			if (i + 1 == cursor)
				printf("  " BOLD CYAN "▶ %s" RESET "\n", project->suites[i].name);
			else
				printf("    %s\n", project->suites[i].name);
			i++;
		}
		printf("\n  " DIM "↑/↓ Navigate   Enter Run   v Verbose   b Back   q Quit" RESET "\n");
		key = read_key();
		if (key == KEY_QUIT)
			return (-3);
		if (key == KEY_BACK)
			return (-2);
		if (key == KEY_VERBOSE)
			*verbose = !(*verbose);
		if (key == KEY_UP && cursor > 0)
			cursor--;
		if (key == KEY_DOWN && cursor < total - 1)
			cursor++;
		if (key == KEY_ENTER)
		{
			if (cursor == 0)
				return (-1);
			return (cursor - 1);
		}
		
	}
}

void	launch_menu(t_project **projects, int count)
{
	int	verbose;
	int	proj_idx;
	int	suite_idx;
	int	running;

	verbose = 0;
	running = 1;
	enable_raw();
	while (running)
	{
		proj_idx = project_menu(projects, count);
		if (proj_idx < 0)
			break ;
		while (running)
		{
			suite_idx = suite_menu(projects[proj_idx], &verbose);
			if (suite_idx == -3)
			{
				running = 0;
				break ;
			}
			if (suite_idx == -2)
				break ;
			restore_terminal();
			printf("\n");
			if (suite_idx == -1)
				run_project_all(projects[proj_idx], verbose);
			else
				run_suite(projects[proj_idx],
					&projects[proj_idx]->suites[suite_idx], verbose);
			printf("\n  " DIM "Press any key to continue..." RESET "\n");
			enable_raw();
			read_key();
		}
	}
	restore_terminal();
	printf("\nGoodbye!\n");
}
