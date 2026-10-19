#include "gnl_tester.h"

int	gnl_open_pipe(const char *content)
{
	int		fds[2];
	size_t	len;

	if (GNL_PIPE(fds) != 0)
		return (-1);
	if (content)
	{
		len = strlen(content);
		if (len > 0)
			write(fds[1], content, len);
	}
	close(fds[1]);
	return (fds[0]);
}

void	gnl_drain(int fd)
{
	char	*line;

	while ((line = get_next_line(fd)) != NULL)
		free(line);
}
