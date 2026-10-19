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

/*
** Open a read-only pipe pre-filled with `content`.
** The write end is closed before returning, so get_next_line() will see EOF
** once it has consumed all the bytes.
** Returns the read-fd on success, -1 on error.
*/
int		gnl_open_pipe(const char *content);

/*
** Read and free every remaining line from fd until get_next_line() returns NULL.
** Use at the end of a test to leave the static variable in a clean state.
*/
void	gnl_drain(int fd);

#endif
