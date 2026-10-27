#ifndef GNL_TESTER_H
# define GNL_TESTER_H

# include "../../include/tester.h"
# include "../../include/weak_functions.h"
# include <stdlib.h>
# include <string.h>

# ifdef _WIN32
#  include <io.h>
#  include <fcntl.h>
#  define GNL_PIPE(fds) _pipe((fds), 65536, _O_BINARY)
# else
#  define GNL_PIPE(fds) pipe(fds)
# endif

/* Set by the Makefile (-D BUFFER_SIZE=n), the same value the project uses. */
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

/* Absolute path of the project, set by the Makefile. */
# ifndef GNL_PATH
#  define GNL_PATH "../get_next_line"
# endif

/*
	The implementation under test. The mandatory and the bonus files are
	compiled separately (the bonus get_next_line is renamed
	get_next_line_bonus), and every suite wrapper in registry.c points g_gnl at
	the one its category tests, so the same suites run against both.
*/
extern char	*(*g_gnl)(int fd);

/*
	Open a read-only pipe pre-filled with `content`.
	The write end is closed before returning, so get_next_line() will see EOF
	once it has consumed all the bytes. A pipe holds at most 64 KiB: use
	gnl_open_file() for bigger contents.
	Returns the read-fd on success, -1 on error.
*/
int		gnl_open_pipe(const char *content);

/*
	Open a regular (temporary, already deleted) file containing `content`,
	positioned at its start. Returns the fd on success, -1 on error.
*/
int		gnl_open_file(const char *content);

/* malloc'd string of `len` times `c`, followed by `end` (may be ""). */
char	*gnl_repeat(char c, size_t len, const char *end);

/*
	Read and free every remaining line from fd until get_next_line() returns NULL.
	Use at the end of a test to leave the static variable in a clean state.
*/
void	gnl_drain(int fd);

#endif
