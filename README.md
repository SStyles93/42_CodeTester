*This repository has been created as part of the 42 curriculum by sstyles.*

# 42 Projects

## Description



## Content

| Folder | What it is | What it is useful for |
| --- | --- | --- |
| [`C_Tester/`](C_Tester/) | An interactive test program for my projects (currently Libft). | Checks every Libft function against expected results, and can look for memory leaks, failed-`malloc` handling and undefined behaviour (AddressSanitizer, Valgrind). |

## Instructions

### Requirements

- A C compiler (`cc` / `gcc`)
- `make`
- A Unix-like environment (Linux or macOS). On Windows, use WSL or MinGW / Git Bash.
- *Optional, for `C_Tester`:* `valgrind` (Linux) for leak checking, and a compiler with
  AddressSanitizer support for `make asan`.

### Compilation

**C_Tester** — builds `Libft` automatically, then the tester:

```sh
cd C_Tester
make                # build ./tester
./tester            # interactive menu
./tester all        # run every test suite
./tester ft_split   # run a single suite
./tester -v all     # verbose output
make asan           # build and run with AddressSanitizer + UBSan
make valgrind       # build and run under Valgrind (Linux)
```
