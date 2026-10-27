#include "gnl_tester.h"

char	*(*g_gnl)(int fd) = get_next_line;

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

/*
	tmpfile() is removed when closed; dup() keeps the file alive and shares
	its offset, so rewinding the FILE also rewinds the returned fd.
*/
int	gnl_open_file(const char *content)
{
	FILE	*f;
	size_t	len;
	int		fd;

	f = tmpfile();
	if (!f)
		return (-1);
	len = strlen(content);
	if (fwrite(content, 1, len, f) != len || fflush(f) != 0)
	{
		fclose(f);
		return (-1);
	}
	rewind(f);
	fd = dup(fileno(f));
	fclose(f);
	return (fd);
}

char	*gnl_repeat(char c, size_t len, const char *end)
{
	char	*s;
	size_t	end_len;

	end_len = strlen(end);
	s = malloc(len + end_len + 1);
	if (!s)
		return (NULL);
	memset(s, c, len);
	memcpy(s + len, end, end_len + 1);
	return (s);
}

void	gnl_drain(int fd)
{
	char	*line;

	while ((line = g_gnl(fd)) != NULL)
		free(line);
}
