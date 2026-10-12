#include "../../include/tester.h"

void	test_ft_isalpha(t_ctx *ctx);
void	test_ft_isdigit(t_ctx *ctx);
void	test_ft_isalnum(t_ctx *ctx);
void	test_ft_isascii(t_ctx *ctx);
void	test_ft_isprint(t_ctx *ctx);
void	test_ft_toupper(t_ctx *ctx);
void	test_ft_tolower(t_ctx *ctx);
void	test_ft_strlen(t_ctx *ctx);
void	test_ft_memset(t_ctx *ctx);
void	test_ft_bzero(t_ctx *ctx);
void	test_ft_memcpy(t_ctx *ctx);
void	test_ft_memmove(t_ctx *ctx);
void	test_ft_memchr(t_ctx *ctx);
void	test_ft_memcmp(t_ctx *ctx);
void	test_ft_strchr(t_ctx *ctx);
void	test_ft_strrchr(t_ctx *ctx);
void	test_ft_strnstr(t_ctx *ctx);
void	test_ft_strncmp(t_ctx *ctx);
void	test_ft_strlcat(t_ctx *ctx);
void	test_ft_strlcpy(t_ctx *ctx);
void	test_ft_atoi(t_ctx *ctx);
void	test_ft_calloc(t_ctx *ctx);
void	test_ft_strdup(t_ctx *ctx);
void	test_ft_substr(t_ctx *ctx);
void	test_ft_strjoin(t_ctx *ctx);
void	test_ft_strtrim(t_ctx *ctx);
void	test_ft_split(t_ctx *ctx);
void	test_ft_itoa(t_ctx *ctx);
void	test_ft_strmapi(t_ctx *ctx);
void	test_ft_striteri(t_ctx *ctx);
void	test_ft_putchar_fd(t_ctx *ctx);
void	test_ft_putstr_fd(t_ctx *ctx);
void	test_ft_putendl_fd(t_ctx *ctx);
void	test_ft_putnbr_fd(t_ctx *ctx);
void	test_ft_lstnew(t_ctx *ctx);
void	test_ft_lstadd_front(t_ctx *ctx);
void	test_ft_lstadd_back(t_ctx *ctx);
void	test_ft_lstsize(t_ctx *ctx);
void	test_ft_lstlast(t_ctx *ctx);
void	test_ft_lstdelone(t_ctx *ctx);
void	test_ft_lstclear(t_ctx *ctx);
void	test_ft_lstiter(t_ctx *ctx);
void	test_ft_lstmap(t_ctx *ctx);
void	test_memory(t_ctx *ctx);

static t_suite	g_suites[] = {
	{"ft_isalpha",      test_ft_isalpha},
	{"ft_isdigit",      test_ft_isdigit},
	{"ft_isalnum",      test_ft_isalnum},
	{"ft_isascii",      test_ft_isascii},
	{"ft_isprint",      test_ft_isprint},
	{"ft_toupper",      test_ft_toupper},
	{"ft_tolower",      test_ft_tolower},
	{"ft_strlen",       test_ft_strlen},
	{"ft_memset",       test_ft_memset},
	{"ft_bzero",        test_ft_bzero},
	{"ft_memcpy",       test_ft_memcpy},
	{"ft_memmove",      test_ft_memmove},
	{"ft_memchr",       test_ft_memchr},
	{"ft_memcmp",       test_ft_memcmp},
	{"ft_strchr",       test_ft_strchr},
	{"ft_strrchr",      test_ft_strrchr},
	{"ft_strnstr",      test_ft_strnstr},
	{"ft_strncmp",      test_ft_strncmp},
	{"ft_strlcat",      test_ft_strlcat},
	{"ft_strlcpy",      test_ft_strlcpy},
	{"ft_atoi",         test_ft_atoi},
	{"ft_calloc",       test_ft_calloc},
	{"ft_strdup",       test_ft_strdup},
	{"ft_substr",       test_ft_substr},
	{"ft_strjoin",      test_ft_strjoin},
	{"ft_strtrim",      test_ft_strtrim},
	{"ft_split",        test_ft_split},
	{"ft_itoa",         test_ft_itoa},
	{"ft_strmapi",      test_ft_strmapi},
	{"ft_striteri",     test_ft_striteri},
	{"ft_putchar_fd",   test_ft_putchar_fd},
	{"ft_putstr_fd",    test_ft_putstr_fd},
	{"ft_putendl_fd",   test_ft_putendl_fd},
	{"ft_putnbr_fd",    test_ft_putnbr_fd},
	{"ft_lstnew",       test_ft_lstnew},
	{"ft_lstadd_front", test_ft_lstadd_front},
	{"ft_lstadd_back",  test_ft_lstadd_back},
	{"ft_lstsize",      test_ft_lstsize},
	{"ft_lstlast",      test_ft_lstlast},
	{"ft_lstdelone",    test_ft_lstdelone},
	{"ft_lstclear",     test_ft_lstclear},
	{"ft_lstiter",      test_ft_lstiter},
	{"ft_lstmap",       test_ft_lstmap},
	{"memory",          test_memory},
};

static t_project	g_project = {
	"Libft",
	g_suites,
	sizeof(g_suites) / sizeof(g_suites[0])
};

t_project	*get_libft_project(void)
{
	return (&g_project);
}
