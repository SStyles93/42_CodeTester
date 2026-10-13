#include "../../include/tester.h"
#include "../../include/weak_functions.h"

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
	{"ft_isalpha",      test_ft_isalpha,      (void *)ft_isalpha,      1},
	{"ft_isdigit",      test_ft_isdigit,      (void *)ft_isdigit,      1},
	{"ft_isalnum",      test_ft_isalnum,      (void *)ft_isalnum,      1},
	{"ft_isascii",      test_ft_isascii,      (void *)ft_isascii,      1},
	{"ft_isprint",      test_ft_isprint,      (void *)ft_isprint,      1},
	{"ft_toupper",      test_ft_toupper,      (void *)ft_toupper,      1},
	{"ft_tolower",      test_ft_tolower,      (void *)ft_tolower,      1},
	{"ft_strlen",       test_ft_strlen,       (void *)ft_strlen,       1},
	{"ft_memset",       test_ft_memset,       (void *)ft_memset,       1},
	{"ft_bzero",        test_ft_bzero,        (void *)ft_bzero,        1},
	{"ft_memcpy",       test_ft_memcpy,       (void *)ft_memcpy,       1},
	{"ft_memmove",      test_ft_memmove,      (void *)ft_memmove,      1},
	{"ft_memchr",       test_ft_memchr,       (void *)ft_memchr,       1},
	{"ft_memcmp",       test_ft_memcmp,       (void *)ft_memcmp,       1},
	{"ft_strchr",       test_ft_strchr,       (void *)ft_strchr,       1},
	{"ft_strrchr",      test_ft_strrchr,      (void *)ft_strrchr,      1},
	{"ft_strnstr",      test_ft_strnstr,      (void *)ft_strnstr,      1},
	{"ft_strncmp",      test_ft_strncmp,      (void *)ft_strncmp,      1},
	{"ft_strlcat",      test_ft_strlcat,      (void *)ft_strlcat,      1},
	{"ft_strlcpy",      test_ft_strlcpy,      (void *)ft_strlcpy,      1},
	{"ft_atoi",         test_ft_atoi,         (void *)ft_atoi,         1},
	{"ft_calloc",       test_ft_calloc,       (void *)ft_calloc,       1},
	{"ft_strdup",       test_ft_strdup,       (void *)ft_strdup,       1},
	{"ft_substr",       test_ft_substr,       (void *)ft_substr,       1},
	{"ft_strjoin",      test_ft_strjoin,      (void *)ft_strjoin,      1},
	{"ft_strtrim",      test_ft_strtrim,      (void *)ft_strtrim,      1},
	{"ft_split",        test_ft_split,        (void *)ft_split,        1},
	{"ft_itoa",         test_ft_itoa,         (void *)ft_itoa,         1},
	{"ft_strmapi",      test_ft_strmapi,      (void *)ft_strmapi,      1},
	{"ft_striteri",     test_ft_striteri,     (void *)ft_striteri,     1},
	{"ft_putchar_fd",   test_ft_putchar_fd,   (void *)ft_putchar_fd,   1},
	{"ft_putstr_fd",    test_ft_putstr_fd,    (void *)ft_putstr_fd,    1},
	{"ft_putendl_fd",   test_ft_putendl_fd,   (void *)ft_putendl_fd,   1},
	{"ft_putnbr_fd",    test_ft_putnbr_fd,    (void *)ft_putnbr_fd,    1},
	{"ft_lstnew",       test_ft_lstnew,       (void *)ft_lstnew,       1},
	{"ft_lstadd_front", test_ft_lstadd_front, (void *)ft_lstadd_front, 1},
	{"ft_lstadd_back",  test_ft_lstadd_back,  (void *)ft_lstadd_back,  1},
	{"ft_lstsize",      test_ft_lstsize,      (void *)ft_lstsize,      1},
	{"ft_lstlast",      test_ft_lstlast,      (void *)ft_lstlast,      1},
	{"ft_lstdelone",    test_ft_lstdelone,    (void *)ft_lstdelone,    1},
	{"ft_lstclear",     test_ft_lstclear,     (void *)ft_lstclear,     1},
	{"ft_lstiter",      test_ft_lstiter,      (void *)ft_lstiter,      1},
	{"ft_lstmap",       test_ft_lstmap,       (void *)ft_lstmap,       1},
	{"memory",          test_memory,		   NULL,				   0},
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
