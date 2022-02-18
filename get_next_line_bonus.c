/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujpark <sujpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 19:43:50 by sujpark           #+#    #+#             */
/*   Updated: 2022/02/18 15:58:50 by sujpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	free_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*separate_line(char **backup)
{
	int		i;
	char	*line;
	char	*old_backup;

	i = 0;
	while ((*backup)[i] != '\n')
		i++;
	old_backup = *backup;
	line = ft_substr(old_backup, 0, i + 1);
	*backup = ft_strdup(&(*backup)[i + 1]);
	free(old_backup);
	return (line);
}

static int	read_file(int fd, char **buffer, char **backup)
{
	int		bytes;
	char	*old_backup;

	bytes = 1;
	while (!ft_strchr(*backup, '\n') && bytes)
	{
		bytes = read(fd, *buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (bytes);
		(*buffer)[bytes] = '\0';
		old_backup = *backup;
		*backup = ft_strjoin(old_backup, *buffer);
		free(old_backup);
	}
	return (bytes);
}

static char	*get_line(int fd, char **buffer, char **backup)
{
	int		bytes;
	char	*last_line;

	bytes = read_file(fd, buffer, backup);
	if (!**backup || bytes == -1)
	{
		free_ptr(backup);
		return (NULL);
	}
	if (ft_strchr(*backup, '\n'))
		return (separate_line(backup));
	last_line = ft_strdup(*backup);
	free_ptr(backup);
	return (last_line);
}

char	*get_next_line(int fd)
{
	static char		*backup[OPEN_MAX + 1];
	char			*buffer;
	char			*result;

	if (fd < 0 || fd > OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!backup[fd])
		backup[fd] = ft_strdup("");
	result = get_line(fd, &buffer, &backup[fd]);
	free(buffer);
	return (result);
}
