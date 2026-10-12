*This repository has been created as part of the 42 curriculum by sstyles.*

# CodeTester

An interactive C test harness for 42 projects. Currently covers the full **Libft** function set, including bonus linked-list functions.

## Features

- **Interactive menu** — browse and run suites from a TUI when launched with no arguments
- **Crash isolation** — each suite runs in a forked child process; a segfault or infinite loop only kills that suite, not the whole run
- **Timeout detection** — tests that hang are killed after a configurable deadline (default 10 s)
- **Malloc-failure testing** — `memcheck` wraps `malloc`/`calloc`/`free` to inject allocation failures and verify your code handles them gracefully (Linux only)
- **AddressSanitizer + UBSan** — `make asan` recompiles everything with sanitizers and runs the full suite
- **Valgrind** — `make valgrind` runs under Valgrind with full leak checking (Linux only)
- **Cross-platform** — builds on Linux, macOS, and Windows (native or WSL/MinGW)

## Requirements

| Tool | Notes |
|---|---|
| `gcc` / `cc` | C compiler |
| `make` | Build system |
| Unix-like environment | Linux or macOS natively; Windows via WSL or MinGW/Git Bash |
| `valgrind` | Optional — Linux only, for `make valgrind` |
| ASan-capable compiler | Optional — for `make asan` |

The tester expects your **Libft** source to live at `../Libft` relative to this repo. It builds `libft.a` automatically before linking.

## Usage

```sh
make              # build ./tester
./tester          # interactive menu (when stdin is a terminal)
./tester all      # run every test suite non-interactively
./tester ft_split # run a single named suite
./tester -v all   # verbose output — shows each individual check
./tester -t 5 all # set timeout to 5 s per suite (default: 10 s)
make asan         # build with AddressSanitizer + UBSan, then run all
make valgrind     # build normally, then run under Valgrind (Linux)
make re           # clean rebuild
```

## Test suites

All 43 standard Libft functions have a dedicated suite, plus a `memory` suite that stress-tests malloc-failure handling:

| Category | Functions |
|---|---|
| Character checks | `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint` |
| Character conversion | `ft_toupper`, `ft_tolower` |
| String | `ft_strlen`, `ft_strchr`, `ft_strrchr`, `ft_strnstr`, `ft_strncmp`, `ft_strlcat`, `ft_strlcpy`, `ft_strdup`, `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_strmapi`, `ft_striteri` |
| Memory | `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp` |
| Conversion | `ft_atoi`, `ft_itoa` |
| Allocation | `ft_calloc` |
| File descriptors | `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd` |
| Linked list (bonus) | `ft_lstnew`, `ft_lstadd_front`, `ft_lstadd_back`, `ft_lstsize`, `ft_lstlast`, `ft_lstdelone`, `ft_lstclear`, `ft_lstiter`, `ft_lstmap` |
| Memory checks | `memory` (malloc-failure injection, Linux) |

## Project structure

```
CodeTester/
├── src/
│   ├── main.c        # Entry point, argument parsing
│   ├── menu.c        # Interactive TUI
│   ├── runner.c      # Fork/isolation, suite runner, timeout
│   ├── utils.c       # check_* assertion helpers
│   └── memcheck.c    # malloc/calloc/free wrapper for failure injection
├── include/
│   └── tester.h      # Shared types and declarations
├── tests/
│   └── libft/
│       ├── registry.c        # Registers all suites into the project
│       └── test_<name>.c     # One file per Libft function
└── Makefile
```
