*This repository has been created to test the 42 School projects.*

> For bugs, missing edge cases, or support: [contact me](mailto:samuel.styles93@gmail.com)

# CodeTester

An interactive C tester for 42 projects.  

## Coverage
| **Library** | **State** | **Grade** |
|---|---|---|
| **Libft** | Full ${\color{lightgreen}\text{(including linked-list functions)}}$ | ${\color{lightgreen}\text{100/100}}$ |  
| **Printf** | Full ${\color{lightgreen}\text{(mandatory + bonus + original printf behaviour)}}$ | ${\color{red}\text{N}}$ |


## Features

- **Interactive menu** — browse and run suites from a TUI when launched with no arguments
- **Crash isolation** — each suite runs in a forked child process; a segfault or infinite loop only kills that suite, not the whole run
- **Timeout detection** — tests that hang are killed after a configurable deadline (default 10 s)
- **Malloc-failure testing** — `memcheck` wraps `malloc`/`calloc`/`free` to inject allocation failures and verify your code handles them gracefully ${\color{orange}\text{(Linux only)}}$
- **AddressSanitizer + UBSan** — `make asan` recompiles everything with sanitizers and runs the full suite ${\color{orange}\text{(Linux and macOS)}}$
- **Valgrind** — `make valgrind` runs under Valgrind with full leak checking ${\color{orange}\text{(Linux only)}}$
- **Cross-platform** — builds on Linux, macOS, and Windows (native or WSL/MinGW)
- **Auto-detection** — the Makefile finds sibling project directories automatically (case-insensitive: `libft`, `Libft`, `LIBFT`, etc.) and only compiles the suites for projects that are present
- **Missing-function detection** — functions are declared weak, so an unimplemented one is reported as `MISSING` instead of breaking the build
- **Compared against the real printf** — every `ft_printf` case runs `snprintf` with the same arguments and compares the bytes written to stdout and the return value
- **Subject checks** — Makefile rules, relinking, library/header names, forbidden external functions and the README format required by the subject
- **Informational categories** — behaviour the subject does not require (e.g. what the original printf does with `%k`) is reported without counting as a failure

## Requirements

| Tool | Notes |
|---|---|
| `gcc` / `cc` | C compiler |
| `make` | Build system |
| Unix-like environment | ${\color{lightgreen}\text{Linux / macOS}}$ natively — ${\color{orange}\text{Windows}}$ via WSL or MinGW/Git Bash |
| `valgrind` | Optional — ${\color{orange}\text{Linux only}}$, for `make valgrind` |
| ASan-capable compiler | Optional — ${\color{orange}\text{Linux and macOS}}$, for `make asan` |

The tester auto-detects your project directories as siblings of this repo (e.g. `../libft`, `../printf`). It builds the relevant library (`libft.a`, `libftprintf.a`) automatically before linking, by running your project's own Makefile.

## Building

Your project directories must be **siblings** of `CodeTester` — the Makefile finds them automatically. Any combination works (only libft, only printf, or both):

```
parent/
├── CodeTester/
├── libft/          ← auto-detected (libft.a)
└── printf/         ← auto-detected (libftprintf.a)
```

> **Printf:** `libftprintf.a` must contain real object files. If your printf uses libft, copy `libft.a` and add your objects to it (`cp $(LIBFT) $(NAME)` then `ar rcs $(NAME) $(OBJS)`). Passing `libft.a` itself to `ar` stores it as a nested archive, which the linker cannot use.

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
./tester all      # run every suite of every detected project
./tester libft    # run one whole project (case-insensitive name)
./tester printf   # run every printf category (mandatory, bonus, original)
./tester printf_bonus  # run a single category
./tester ft_split # run a single named suite (searched in every project)
./tester -v printf_s   # verbose: each check, with expected/got on failure
./tester -v all   # verbose output — shows each individual check
./tester -t 5 all # set timeout to 5 s per suite (default: 10 s)
make asan         # build with AddressSanitizer + UBSan, then run all
make valgrind     # build normally, then run under Valgrind (Linux)
make re           # clean rebuild
```

## Test suites

### Libft

All 43 standard Libft functions have a dedicated suite, plus a `memory` suite that checks the allocating functions for leaks:

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
| Memory checks | `memory` (leak check, Linux) |

### ft_printf

Every case calls the real `snprintf` and `ft_printf` with the same arguments, captures what `ft_printf` writes to fd 1, and compares the bytes and the return value. The reference is therefore the libc of the machine running the tests (glibc on Linux/WSL, libSystem on macOS): `(null)`, `(nil)` and `0x0` are expected where the host printf prints them.

The suites are split into three categories, following the subject:

**Printf_mandatory** — the conversions `cspdiuxX%` without any flag, and the subject requirements

| Suite | What it covers |
|---|---|
| `ft_printf` | plain text, empty format, every conversion in one call, 20 arguments, a 20 000-char format |
| `printf_c` | `'\0'` (written and counted), values > 127, negative and > 255 |
| `printf_s` | empty string, conversion characters inside the argument, high bytes, `NULL`, a 100 000-char argument |
| `printf_p` | `NULL`, `1`, `ULONG_MAX`, `LONG_MIN`, values above 32 bits, a real address |
| `printf_di` / `printf_u` / `printf_x` | limits, digit-count transitions (9→10, 15→16), negatives reinterpreted as unsigned |
| `printf_percent` | `%%`, consecutive `%%`, `%%` before text and before a conversion |
| `printf_behavior` | no buffering (output interleaved in order with raw `write()`), `-1` when `write` fails (stdout closed), recovery after an error, no state kept between calls, no leaks and malloc-failure handling ${\color{orange}\text{(Linux)}}$ |
| `printf_project` | `libftprintf.a` at the root, `ft_printf.h` with the prototype, Makefile `NAME` / `all` / `clean` / `fclean` / `re`, `ar` used and not `libtool`, `-Wall -Wextra -Werror`, no relink, only `malloc` / `free` / `write` as external functions (read with `nm`), README first line and sections |

**Printf_bonus** — flags `-0.`, minimum field width and `# +`

| Suite | What it covers |
|---|---|
| `bonus_minus_zero_dot` | width, `-`, `0`, `.`: `-` overriding `0`, precision disabling `0`, `%.0d` with 0, `NULL` with a precision, widths up to 100 000 |
| `bonus_hash_space_plus` | `#`, `' '`, `+`: `%#x` with 0, `#` with `0` and a precision, `+` overriding `' '` in either order |
| `bonus_combinations` | flags, width and precision must not leak into the next conversion or call |
| `bonus_project` | Makefile `bonus` rule, `make bonus` does not relink |

**Printf_original** ${\color{orange}\text{(informational — never counted as failures)}}$ — what the original printf does where the C standard says the behaviour is undefined. The subject does not require it; it is shown as `~ n/m match` in yellow.

| Suite | What it covers |
|---|---|
| `original_conversions` | incomplete or unknown conversions without flags: `"%"`, `"abc%"`, `%k`, `ft_printf(NULL)` |
| `original_flags` | the same with flags (`%5%`, `%-5k`) and flags that do not apply to the conversion (`%#d`, `%+u`, `%05s`, `%.3c`, `%+p`…) |

Cases that may crash (`NULL` format, trailing `%`) run in their own process, so a crash only fails that case.

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
│       ├── registry.c  # Registers the Printf_mandatory / _bonus / _original categories
│       ├── pf_tester.h # PF / PF_L macros: compare ft_printf with snprintf
│       ├── pf_utils.c  # stdout capture into a temp file, result reporting
│       ├── test_ft_printf.c, test_pf_<c|s|p|di|u|x|percent>.c  # mandatory conversions
│       ├── test_pf_behavior.c   # buffering, write errors, memory
│       ├── test_pf_project.c    # subject requirements (Makefile, nm, README...)
│       ├── test_pf_minus_zero_dot.c, test_pf_hash_space_plus.c,
│       │   test_pf_combinations.c  # bonus flags
│       └── test_pf_original.c   # undefined behaviour vs the original printf
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

#### ft_printf cases

In `tests/printf/`, use the `PF` macro from `pf_tester.h` instead of the `check_*` helpers. It runs `snprintf` and `ft_printf` with the same arguments and compares the output and return value; the label is the call's source text:

```c
// tests/printf/test_pf_x.c
PF(ctx, "%x", 255u);
PF(ctx, "[%-8.3d]", -42);
```

For a format built at runtime, use `PF_L(ctx, label, format, args...)` to give it a readable label. Put a case in the category it belongs to: no flags → mandatory, flags → bonus, undefined behaviour → original.

---

### Adding a new suite to an existing project

This takes four steps, shown here for adding `ft_foo` to the Libft project:

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

`registry.c` must define a `get_<project>_project()` function. Suite entries take four fields — name, test function, function pointer for presence-checking, and the check flag (`NULL, 0` for a suite that does not test one function, like `printf_project`). A project takes four fields — name, suites, suite count, and the `informational` flag (`1`: failures are shown as differences and never counted, like `Printf_original`):

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
    sizeof(g_suites) / sizeof(g_suites[0]),
    0
};

t_project   *get_<project>_project(void)
{
    return (&g_project);
}
```

A project can be split into several categories by defining one `t_project` per category, as `tests/printf/registry.c` does. Name them `<Project>_<category>`: `./tester <project>` then runs all of them.

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

Because the tester only references your functions **weakly**, the linker does not pull them out of a static archive by itself. Force them in, as the existing blocks do:

- `-Wl,--whole-archive lib.a -Wl,--no-whole-archive` (`-Wl,-all_load` on macOS) keeps every object of the archive (libft block);
- `-Wl,-u,ft_foo lib.a` (`-Wl,-u,_ft_foo` on macOS) pulls in only `ft_foo` and what it needs (printf block). Use this one when the archive contains another project's objects (e.g. `libftprintf.a` contains libft), otherwise the symbols are defined twice.
