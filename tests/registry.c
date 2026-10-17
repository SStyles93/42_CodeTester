#include "../include/tester.h"

#ifdef WITH_LIBFT
t_project	*get_libft_project(void);
#endif
#ifdef WITH_PRINTF
t_project	*get_printf_project(void);
t_project	*get_printf_bonus_project(void);
t_project	*get_printf_original_project(void);
#endif

static t_project	*g_projects[8];

t_project	**get_all_projects(int *count)
{
	int	i;

	i = 0;
#ifdef WITH_LIBFT
	g_projects[i++] = get_libft_project();
#endif
#ifdef WITH_PRINTF
	g_projects[i++] = get_printf_project();
	g_projects[i++] = get_printf_bonus_project();
	g_projects[i++] = get_printf_original_project();
#endif
	*count = i;
	return (g_projects);
}
