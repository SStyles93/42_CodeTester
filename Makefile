# ── Platform detection ───────────────────────────────────────────────────────
ifeq ($(OS),Windows_NT)
    EXE     = .exe
    LFLAGS  = -lkernel32
else
    EXE     =
    LFLAGS  =
endif

# Colour defines (ANSI)
GREEN =\033[0;32m #Light Green
NC=\033[0m #No Colour

NAME        = tester$(EXE)
CC          = gcc
CFLAGS      = -Wall -Wextra
INCLUDES    = -I include

# ── platform ─────────────────────────────────────────────────────────────────
UNAME_S     := $(shell uname -s 2>/dev/null)

# ── auto-detect student project directories ──────────────────────────────────
LIBFT_DIR   := $(or $(wildcard ../libft),$(wildcard ../Libft),$(wildcard ../LIBFT))
PRINTF_DIR  := $(or $(wildcard ../printf),$(wildcard ../Printf),$(wildcard ../PRINTF))
GNL_DIR     := $(or $(wildcard ../get_next_line),$(wildcard ../gnl),$(wildcard ../GNL),$(wildcard ../Get_next_line))

$(info Detected:$(if $(LIBFT_DIR), libft ($(LIBFT_DIR)))$(if $(PRINTF_DIR), printf ($(PRINTF_DIR)))$(if $(GNL_DIR), gnl ($(GNL_DIR))))

# ── tester sources ──────────────────────────────────────────────────────────
SRCS        = src/main.c \
              src/menu.c \
              src/runner.c \
              src/utils.c \
              src/memcheck.c \
              src/readcheck.c \
              tests/registry.c



# ─────────────────────────────────────────────────────────────
# ──────────────────────── LIBFT BLOCK ────────────────────────
# ─────────────────────────────────────────────────────────────

ifneq ($(LIBFT_DIR),)
    LIBFT_A  = $(LIBFT_DIR)/libft.a
    CFLAGS   += -DWITH_LIBFT
    INCLUDES += -I $(LIBFT_DIR)
    ifeq ($(UNAME_S),Darwin)
        LINK_LIBS = -Wl,-all_load $(LIBFT_A)
    else
        LINK_LIBS = -Wl,--whole-archive $(LIBFT_A) -Wl,--no-whole-archive
    endif
    LINK_DEPS = $(LIBFT_A)
    LIBFT_FCLEAN = @$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

    # TEST TO ADD HERE IF NECESSARY
    SRCS     += tests/libft/registry.c \
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
else
    LINK_LIBS    =
    LINK_DEPS    =
    LIBFT_FCLEAN = @true
endif



# ─────────────────────────────────────────────────────────────
# ────────────────────── PRINTFT BLOCK ────────────────────────
# ─────────────────────────────────────────────────────────────

ifneq ($(PRINTF_DIR),)
    PRINTF_A     = $(PRINTF_DIR)/libftprintf.a
    CFLAGS       += -DWITH_PRINTF -DPRINTF_PATH='"$(abspath $(PRINTF_DIR))"'
    INCLUDES     += -I $(PRINTF_DIR)
    # ft_printf is only referenced weakly, which does not pull it out of the
    # archive: force it with -u. Not --whole-archive, because libftprintf.a
    # also contains libft and would clash with ../libft/libft.a.
    ifeq ($(UNAME_S),Darwin)
        LINK_LIBS += -Wl,-u,_ft_printf $(PRINTF_A)
    else
        LINK_LIBS += -Wl,-u,ft_printf $(PRINTF_A)
    endif
    LINK_DEPS    += $(PRINTF_A)
    PRINTF_FCLEAN = @$(MAKE) --no-print-directory -C $(PRINTF_DIR) fclean
    
    # TEST TO ADD HERE IF NECESSARY
    SRCS         += tests/printf/registry.c \
                    tests/printf/pf_utils.c \
                    tests/printf/test_ft_printf.c \
                    tests/printf/test_pf_c.c \
                    tests/printf/test_pf_s.c \
                    tests/printf/test_pf_p.c \
                    tests/printf/test_pf_di.c \
                    tests/printf/test_pf_u.c \
                    tests/printf/test_pf_x.c \
                    tests/printf/test_pf_percent.c \
                    tests/printf/test_pf_minus_zero_dot.c \
                    tests/printf/test_pf_hash_space_plus.c \
                    tests/printf/test_pf_combinations.c \
                    tests/printf/test_pf_behavior.c \
                    tests/printf/test_pf_original.c \
                    tests/printf/test_pf_project.c
else
    PRINTF_FCLEAN = @true
endif

# ─────────────────────────────────────────────────────────────
# ──────────────────────── GNL BLOCK ──────────────────────────
# ─────────────────────────────────────────────────────────────

ifneq ($(GNL_DIR),)
    GNL_MAN_SRCS := $(wildcard $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c)
    GNL_BON_SRCS := $(wildcard $(GNL_DIR)/get_next_line_bonus.c $(GNL_DIR)/get_next_line_utils_bonus.c)

    ifneq ($(GNL_MAN_SRCS)$(GNL_BON_SRCS),)
        # `make re BUFFER_SIZE=1` (or `make gnl-sizes`) to test other values.
        BUFFER_SIZE  ?= 42
        GNL_BS_STAMP  = .gnl_bs_$(BUFFER_SIZE)
        GNL_CFLAGS    = -DBUFFER_SIZE=$(BUFFER_SIZE) -I $(GNL_DIR)
        CFLAGS       += -DWITH_GNL -DGNL_PATH='"$(abspath $(GNL_DIR))"'

        # Mandatory and bonus both define get_next_line (and often the same
        # helpers): each set is linked into one object that only exports its
        # get_next_line, the bonus one renamed get_next_line_bonus, so both
        # are tested in the same binary. The objects stay in obj/gnl, not in
        # the project directory.
        GNL_MAN_OBJS  = $(patsubst $(GNL_DIR)/%.c,obj/gnl/mandatory/%.o,$(GNL_MAN_SRCS))
        GNL_BON_OBJS  = $(patsubst $(GNL_DIR)/%.c,obj/gnl/bonus/%.o,$(GNL_BON_SRCS))
        GNL_OBJS      = $(if $(GNL_MAN_SRCS),obj/gnl/gnl_mandatory.o) \
                        $(if $(GNL_BON_SRCS),obj/gnl/gnl_bonus.o)

        # TEST TO ADD HERE IF NECESSARY
        GNL_TEST_SRCS = tests/gnl/registry.c \
                        tests/gnl/gnl_utils.c \
                        tests/gnl/test_gnl.c \
                        tests/gnl/test_gnl_lines.c \
                        tests/gnl/test_gnl_edge.c \
                        tests/gnl/test_gnl_sizes.c \
                        tests/gnl/test_gnl_sources.c \
                        tests/gnl/test_gnl_memory.c \
                        tests/gnl/test_gnl_project.c \
                        tests/gnl/test_gnl_bonus.c
        SRCS         += $(GNL_TEST_SRCS)
    endif
endif



# ─────────────────────────────────────────────────────────────
# ────────────────────── OTHER BLOCK ──────────────────────────
# ─────────────────────────────────────────────────────────────

# ifneq ($(OTHER_DIR),)
#     OTHER_A     = $(OTHER_DIR)/libname.a
#     CFLAGS       += -DWITH_OTHER
#     INCLUDES     += -I $(OTHER_DIR)
#     ifeq ($(UNAME_S),Darwin)
#       LINK_LIBS += -Wl,-all_load $(OTHER_DIR)
#     else
#       LINK_LIBS += -Wl,--whole-archive $(OTHER_DIR) -Wl,--no-whole-archive
#     endif
#     LINK_DEPS    += $(OTHER_A)
#     OTHER_FCLEAN = @$(MAKE) --no-print-directory -C $(OTHER_DIR) fclean
    
#     # TEST TO ADD HERE IF NECESSARY
#     SRCS         += tests/OTHER/registry.c \
#                     tests/OTHER/test_ft_printf.c
# else
#     OTHER_FCLEAN = @true
# endif



OBJS        = $(SRCS:.c=.o) $(GNL_OBJS)

# ── memory checking ─────────────────────────────────────────────────────────
# Linux: route malloc/calloc/free through src/memcheck.c (leak and
# malloc-failure tests in the "memory" suite). Needs GNU ld's --wrap.
ifeq ($(UNAME_S),Linux)
    CFLAGS  += -DMEMCHECK_WRAP -DREADCHECK_WRAP
    LFLAGS  += -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=free \
               -Wl,--wrap=read
endif

# `make asan` (or SANITIZE=1): build libft and the tester with
# AddressSanitizer + UBSan. Switching mode rebuilds everything, because
# sanitized and plain objects cannot be linked together.
ifeq ($(SANITIZE),1)
    SANFLAGS    = -fsanitize=address,undefined -fno-omit-frame-pointer -g
    CFLAGS      += $(SANFLAGS)
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

$(NAME): $(MODE_STAMP) $(LINK_DEPS) $(OBJS)
	$(CC) $(OBJS) $(LINK_LIBS) $(LFLAGS) -o $(NAME)
	@echo "\n────────────────────────────────────────────────" \
	"\n    Build complete. Run with: $(GREEN)make run$(NC) or $(GREEN)./$(NAME)$(NC)" \
	"\n────────────────────────────────────────────────\n"
    

ifneq ($(LIBFT_DIR),)
# Always ask libft's Makefile, so edits to libft sources are picked up.
$(LIBFT_A): FORCE
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) all $(LIBFT_FLAGS)
endif

ifneq ($(PRINTF_DIR),)
$(PRINTF_A): FORCE
	@$(MAKE) --no-print-directory -C $(PRINTF_DIR) all
endif

# ifneq ($(OTHER_DIR),)
# $(OTHER_A): FORCE
# 	@$(MAKE) --no-print-directory -C $(OTHER_DIR) all
# endif

$(MODE_STAMP):
	@rm -f .build_mode_*
	$(LIBFT_FCLEAN)
	$(PRINTF_FCLEAN)
    #$(OTHER_FCLEAN)
	@rm -f $(OBJS) $(NAME)
	@rm -rf obj
	@touch $@

# `make run` builds if needed, then runs the tester.
# Extra arguments go through ARGS: make run ARGS="-v printf"
run: all
	./$(NAME) $(ARGS)

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

ifneq ($(GNL_OBJS),)
# Rebuild the GNL objects (and their tests) when BUFFER_SIZE changes.
$(GNL_BS_STAMP):
	@rm -f .gnl_bs_*
	@touch $@

$(GNL_TEST_SRCS:.c=.o): CFLAGS += -DBUFFER_SIZE=$(BUFFER_SIZE)
$(GNL_TEST_SRCS:.c=.o): $(GNL_BS_STAMP)

obj/gnl/mandatory/%.o: $(GNL_DIR)/%.c $(GNL_BS_STAMP)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(GNL_CFLAGS) -c $< -o $@

obj/gnl/bonus/%.o: $(GNL_DIR)/%.c $(GNL_BS_STAMP)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(GNL_CFLAGS) -Dget_next_line=get_next_line_bonus -c $< -o $@

# $(call gnl_link,exported symbol): every other symbol of the set becomes local.
ifeq ($(UNAME_S),Darwin)
gnl_link = ld -r -exported_symbol _$(1) -o $@ $^
else
gnl_link = ld -r -o $@.tmp $^ && objcopy --keep-global-symbol=$(1) $@.tmp $@ \
	&& rm -f $@.tmp
endif

obj/gnl/gnl_mandatory.o: $(GNL_MAN_OBJS)
	$(call gnl_link,get_next_line)

obj/gnl/gnl_bonus.o: $(GNL_BON_OBJS)
	$(call gnl_link,get_next_line_bonus)
endif

# `make gnl-sizes`: run the GNL suites with the buffer sizes the subject asks
# about ("Does your function still work if the BUFFER_SIZE value is 9999? If
# it is 1? 10000000?").
gnl-sizes:
	@for bs in 1 42 9999 10000000; do \
		printf "\n──────────── BUFFER_SIZE=%s ────────────\n" $$bs; \
		$(MAKE) --no-print-directory BUFFER_SIZE=$$bs all >/dev/null \
			&& ./$(NAME) gnl || exit 1; \
	done

clean:
	rm -f $(OBJS)
	rm -rf obj

fclean: clean
	rm -f $(NAME) .build_mode_* .gnl_bs_*
	$(LIBFT_FCLEAN)
	$(PRINTF_FCLEAN)
    #$(OTHER_FCLEAN)

re: fclean all

FORCE:

.PHONY: all clean fclean re run asan valgrind gnl-sizes FORCE
