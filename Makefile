# ── Platform detection ───────────────────────────────────────────────────────
ifeq ($(OS),Windows_NT)
    EXE     = .exe
    LFLAGS  = -lkernel32
else
    EXE     =
    LFLAGS  =
endif

NAME        = tester$(EXE)
CC          = gcc
CFLAGS      = -Wall -Wextra
INCLUDES    = -I include -I $(LIBFT_DIR)

LIBFT_DIR   = ../Libft
LIBFT_A     = $(LIBFT_DIR)/libft.a

# ── tester sources ──────────────────────────────────────────────────────────
SRCS        = src/main.c \
              src/menu.c \
              src/runner.c \
              src/utils.c \
              src/memcheck.c \
              tests/libft/registry.c \
              tests/libft/test_isalpha.c \
              tests/libft/test_isdigit.c \
              tests/libft/test_isalnum.c \
              tests/libft/test_isascii.c \
              tests/libft/test_isprint.c \
              tests/libft/test_toupper.c \
              tests/libft/test_tolower.c \
              tests/libft/test_strlen.c \
              tests/libft/test_memset.c \
              tests/libft/test_bzero.c \
              tests/libft/test_memcpy.c \
              tests/libft/test_memmove.c \
              tests/libft/test_memchr.c \
              tests/libft/test_memcmp.c \
              tests/libft/test_strchr.c \
              tests/libft/test_strrchr.c \
              tests/libft/test_strnstr.c \
              tests/libft/test_strncmp.c \
              tests/libft/test_strlcat.c \
              tests/libft/test_strlcpy.c \
              tests/libft/test_atoi.c \
              tests/libft/test_calloc.c \
              tests/libft/test_strdup.c \
              tests/libft/test_substr.c \
              tests/libft/test_strjoin.c \
              tests/libft/test_strtrim.c \
              tests/libft/test_split.c \
              tests/libft/test_itoa.c \
              tests/libft/test_strmapi.c \
              tests/libft/test_striteri.c \
              tests/libft/test_putchar_fd.c \
              tests/libft/test_putstr_fd.c \
              tests/libft/test_putendl_fd.c \
              tests/libft/test_putnbr_fd.c \
              tests/libft/test_lstnew.c \
              tests/libft/test_lstadd_front.c \
              tests/libft/test_lstadd_back.c \
              tests/libft/test_lstsize.c \
              tests/libft/test_lstlast.c \
              tests/libft/test_lstdelone.c \
              tests/libft/test_lstclear.c \
              tests/libft/test_lstiter.c \
              tests/libft/test_lstmap.c \
              tests/libft/test_memory.c

# ── bonus Libft sources ─────
# Flags: -std=gnu89 silences the missing-return-type error in ft_strtrim.c;
#        -w suppresses all other warnings from buggy student code;
#        -Dft_strdup=strdup fixes the mismatch between ft_strmapi.c calling
#        ft_strdup() and ft_strdup.c actually defining strdup().
BONUS_DIR   = $(LIBFT_DIR)
BONUS_FLAGS = -std=gnu89 -w -I $(LIBFT_DIR) -Dft_strdup=strdup
BONUS_SRCS  = $(BONUS_DIR)/ft_strdup.c \
              $(BONUS_DIR)/ft_substr.c \
              $(BONUS_DIR)/ft_strjoin.c \
              $(BONUS_DIR)/ft_strtrim.c \
              $(BONUS_DIR)/ft_split.c \
              $(BONUS_DIR)/ft_itoa.c \
              $(BONUS_DIR)/ft_strmapi.c \
              $(BONUS_DIR)/ft_striteri.c \
              $(BONUS_DIR)/ft_putchar_fd.c \
              $(BONUS_DIR)/ft_putstr_fd.c \
              $(BONUS_DIR)/ft_putendl_fd.c \
              $(BONUS_DIR)/ft_putnbr_fd.c

BONUS_OBJS  = $(BONUS_SRCS:$(BONUS_DIR)/%.c=tests/libft/bonus_%.o)

OBJS        = $(SRCS:.c=.o)

# ── memory checking ─────────────────────────────────────────────────────────
# Linux: route malloc/calloc/free through src/memcheck.c (leak and
# malloc-failure tests in the "memory" suite). Needs GNU ld's --wrap.
UNAME_S     := $(shell uname -s 2>/dev/null)
ifeq ($(UNAME_S),Linux)
    CFLAGS  += -DMEMCHECK_WRAP
    LFLAGS  += -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=free
endif

# `make asan` (or SANITIZE=1): build libft and the tester with
# AddressSanitizer + UBSan. Switching mode rebuilds everything, because
# sanitized and plain objects cannot be linked together.
ifeq ($(SANITIZE),1)
    SANFLAGS    = -fsanitize=address,undefined -fno-omit-frame-pointer -g
    CFLAGS      += $(SANFLAGS)
    BONUS_FLAGS += $(SANFLAGS)
    LFLAGS      += $(SANFLAGS)
    LIBFT_FLAGS = CFLAGS="-Wall -Wextra -Werror -c $(SANFLAGS)"
    MODE        = asan
else
    MODE        = normal
endif
MODE_STAMP  = .build_mode_$(MODE)

VALGRIND    = valgrind -q --leak-check=full \
              --errors-for-leak-kinds=definite,indirect --error-exitcode=42

# ── rules ────────────────────────────────────────────────────────────────────
all: $(NAME)

$(NAME): $(MODE_STAMP) $(LIBFT_A) $(OBJS) $(BONUS_OBJS)
	$(CC) $(OBJS) $(BONUS_OBJS) $(LIBFT_A) $(LFLAGS) -o $(NAME)
	@echo "Build complete. Run with: ./tester"

# Always ask libft's Makefile, so edits to libft sources are picked up.
$(LIBFT_A): FORCE
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) all $(LIBFT_FLAGS)

$(MODE_STAMP):
	@rm -f .build_mode_*
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean
	@rm -f $(OBJS) $(BONUS_OBJS) $(NAME)
	@touch $@

asan:
	@$(MAKE) --no-print-directory SANITIZE=1 all
	./$(NAME) all

valgrind:
	@command -v valgrind >/dev/null || { echo "valgrind not found:" \
		"sudo apt install valgrind"; exit 1; }
	@$(MAKE) --no-print-directory all
	$(VALGRIND) ./$(NAME) -t 60 all

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Bonus files get a separate, more permissive compile rule
tests/libft/bonus_%.o: $(BONUS_DIR)/%.c
	$(CC) $(BONUS_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME) .build_mode_*

re: fclean all

FORCE:

.PHONY: all clean fclean re asan valgrind FORCE
