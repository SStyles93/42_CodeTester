*This repository has been created to test the 42 School projects.*

# CodeTester

An interactive C tester for 42 projects.  

## Coverage
| **Library** | **State** | **Grade** |
|---|---|---|
| **Libft** | Full ${\color{lightgreen}\text{(including linked-list functions)}}$ | ${\color{lightgreen}\text{100/100}}$ |  
**Printf** | ${\color{orange}\text{In progress}}$ | ${\color{red}\text{N}}$ |


## Features

- **Interactive menu** — browse and run suites from a TUI when launched with no arguments
- **Crash isolation** — each suite runs in a forked child process; a segfault or infinite loop only kills that suite, not the whole run
- **Timeout detection** — tests that hang are killed after a configurable deadline (default 10 s)
- **Malloc-failure testing** — `memcheck` wraps `malloc`/`calloc`/`free` to inject allocation failures and verify your code handles them gracefully ${\color{orange}\text{(Linux only)}}$
- **AddressSanitizer + UBSan** — `make asan` recompiles everything with sanitizers and runs the full suite ${\color{orange}\text{(Linux and macOS)}}$
- **Valgrind** — `make valgrind` runs under Valgrind with full leak checking ${\color{orange}\text{(Linux only)}}$
- **Cross-platform** — builds on Linux, macOS, and Windows (native or WSL/MinGW)
- **Auto-detection** — the Makefile finds sibling project directories automatically (case-insensitive: `libft`, `Libft`, `LIBFT`, etc.) and only compiles the suites for projects that are present

## Requirements

| Tool | Notes |
|---|---|
| `gcc` / `cc` | C compiler |
| `make` | Build system |
| Unix-like environment | ${\color{lightgreen}\text{Linux / macOS}}$ natively — ${\color{orange}\text{Windows}}$ via WSL or MinGW/Git Bash |
| `valgrind` | Optional — ${\color{orange}\text{Linux only}}$, for `make valgrind` |
| ASan-capable compiler | Optional — ${\color{orange}\text{Linux and macOS}}$, for `make asan` |

The tester auto-detects your project directories as siblings of this repo (e.g. `../libft`). It builds the relevant library (e.g. `libft.a`) automatically before linking.

## Building

Your project directory must be a **sibling** of `CodeTester` — the Makefile finds it automatically:

```
parent/
├── CodeTester/
└── libft/          ← auto-detected
```

### ${\color{lightgreen}\text{macOS}}$ — fully supported

Install Xcode Command Line Tools once, then build:

```sh
xcode-select --install   # one-time setup
make
./tester
```

`make asan` works. `make valgrind` is ${\color{red}\text{not available}}$ on macOS.

### ${\color{orange}\text{Windows}}$

**Option 1 — WSL** ${\color{lightgreen}\text{(recommended — full feature parity)}}$

Install WSL2 with Ubuntu, then build exactly like Linux:

```sh
wsl
make
./tester
```

All features work: `make valgrind`, `make asan`, and malloc-failure injection.

**Option 2 — MinGW / Git Bash** ${\color{orange}\text{(limited)}}$

Install [MinGW-w64](https://www.mingw-w64.org/) or [MSYS2](https://www.msys2.org/), then from Git Bash:

```sh
make
./tester.exe
```

The Makefile detects Windows and produces `tester.exe`. ${\color{red}\text{make valgrind}}$, ${\color{red}\text{make asan}}$, and ${\color{red}\text{malloc-failure tests}}$ are not available in this mode.

---

## Usage

```sh
make              # build ./tester (only suites for detected projects)
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
│   ├── main.c          # Entry point, argument parsing
│   ├── menu.c          # Interactive TUI
│   ├── runner.c        # Fork/isolation, suite runner, timeout
│   ├── utils.c         # check_* assertion helpers
│   └── memcheck.c      # malloc/calloc/free wrapper for failure injection
├── include/
│   ├── tester.h        # Shared types and declarations
│   └── weak_functions.h  # Weak declarations for all project functions
├── tests/
│   ├── registry.c      # Aggregates all project registries (top-level)
│   ├── libft/
│   │   ├── registry.c  # Registers all Libft suites
│   │   └── test_<name>.c  # One file per Libft function
│   └── printf/
│       ├── registry.c  # Registers all ft_printf suites (WIP)
│       └── test_<name>.c  # One file per ft_printf specifier/feature
└── Makefile
```

---

## To contribute

### Adding checks to an existing suite

Each suite lives in `tests/<project>/test_<name>.c`. Open the relevant file and call any of the `check_*` helpers inside the `test_<name>` function:

```c
// tests/libft/test_strlen.c
void test_ft_strlen(t_ctx *ctx)
{
    check_size(ctx, "empty string", 0, ft_strlen(""));
    check_size(ctx, "your new case", 3, ft_strlen("abc")); // add here
}
```

Available helpers (declared in `include/tester.h`):

| Helper | Use for |
|---|---|
| `check_bool(ctx, label, expected, got)` | boolean / int comparisons |
| `check_int(ctx, label, expected, got)` | signed integers |
| `check_uint(ctx, label, expected, got)` | unsigned integers |
| `check_size(ctx, label, expected, got)` | `size_t` values |
| `check_char(ctx, label, expected, got)` | single characters |
| `check_str(ctx, label, expected, got)` | null-terminated strings |
| `check_mem(ctx, label, expected, got, n)` | raw memory buffers |
| `check_ptr(ctx, label, expected, got)` | pointer equality |
| `check_not_null(ctx, label, got)` | pointer non-null assertion |

No rebuild configuration needed — just edit the `.c` file and `make`.

---

### Adding a new suite to an existing project

This takes three steps, shown here for adding `ft_foo` to the Libft project:

**1. Create `tests/libft/test_foo.c`**

Include `weak_functions.h` instead of forward-declaring the function — the weak header centralises all declarations and enables missing-function detection:

```c
#include "../../include/tester.h"
#include "../../include/weak_functions.h"

void    test_ft_foo(t_ctx *ctx)
{
    check_str(ctx, "basic case", "expected", ft_foo("input"));
    // add more check_* calls here
}
```

**2. Add a weak declaration to `include/weak_functions.h`**

```c
__attribute__((weak)) char  *ft_foo(const char *s);
```

This lets the runner detect when the function is absent (NULL weak symbol) and report it as `MISSING` rather than crashing.

**3. Register the suite in `tests/libft/registry.c`**

Add a forward declaration at the top and an entry in `g_suites[]`. The entry now takes four fields — name, test function, pointer to the libft function (for presence-checking), and a `1` flag to enable that check:

```c
void    test_ft_foo(t_ctx *ctx);   // add with the other declarations

static t_suite g_suites[] = {
    // ... existing entries ...
    {"ft_foo", test_ft_foo, (void *)ft_foo, 1},  // add at the end
};
```

**4. Add the file to the Makefile**

Inside the `ifneq ($(LIBFT_DIR),)` block, append to `SRCS`:

```makefile
SRCS += tests/libft/test_foo.c
```

Then `make re` and run `./tester ft_foo` to verify.

---

### Adding a new project

To add tests for a completely new 42 project (e.g. `ft_printf`):

**1. Add weak declarations to `include/weak_functions.h`**

Add a labelled section for the new project's functions:

```c
/* ── MyProject ── */
__attribute__((weak)) char  *ft_foo(const char *s);
```

**2. Create the project directory and files**

```
tests/<project>/
├── registry.c       # mirrors tests/libft/registry.c in structure
└── test_<name>.c    # one file per function/feature being tested
```

`test_<name>.c` includes `weak_functions.h` — no manual forward declarations needed:

```c
#include "../../include/tester.h"
#include "../../include/weak_functions.h"

void    test_ft_foo(t_ctx *ctx)
{
    check_str(ctx, "basic case", "expected", ft_foo("input"));
}
```

`registry.c` must define a `get_<project>_project()` function. Suite entries take four fields — name, test function, function pointer for presence-checking, and the check flag:

```c
#include "../../include/tester.h"
#include "../../include/weak_functions.h"

void    test_ft_foo(t_ctx *ctx);

static t_suite g_suites[] = {
    {"ft_foo", test_ft_foo, (void *)ft_foo, 1},
};

static t_project g_project = {
    "<ProjectName>",
    g_suites,
    sizeof(g_suites) / sizeof(g_suites[0])
};

t_project   *get_<project>_project(void)
{
    return (&g_project);
}
```

**3. Register the project in `tests/registry.c`**

```c
#ifdef WITH_MYPROJECT
t_project   *get_myproject_project(void);
#endif

// inside get_all_projects():
#ifdef WITH_MYPROJECT
    g_projects[i++] = get_myproject_project();
#endif
```

**4. Wire it up in the Makefile**

Add a sibling-directory detection block and a conditional `SRCS +=` block following the same pattern as the existing `LIBFT_DIR` / `PRINTF_DIR` blocks.
