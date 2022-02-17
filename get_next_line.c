/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujpark <sujpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 16:51:24 by sujpark           #+#    #+#             */
/*   Updated: 2022/02/17 16:33:46 by sujpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	free_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*extract_line(char **buffer_backup)
{
	int		i;
	char	*line;
	char	*temp_free;

	i = 0;
	while ((*buffer_backup)[i] != '\0' && (*buffer_backup)[i] != '\n')
		i++;
	temp_free = *buffer_backup;
	line = ft_substr(temp_free, 0, i + 1);
	*buffer_backup = ft_strdup(&(*buffer_backup)[i + 1]);
	free_ptr(&temp_free);
	return (line);
}

static int	read_file(int fd, char **buffer, char **buffer_backup) //backup 에 \n 없는 상태
{
	int		bytes_read;
	char	*temp_free;

	bytes_read = 1;
	while (!ft_strchr(*buffer_backup, '\n') && bytes_read) //backup에 \n 들어가거나 \0 까지 온게 아닌 이상 계속 돌아감
	{
		bytes_read = read(fd, *buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (bytes_read);
		(*buffer)[bytes_read] = '\0'; //buffer 마지막에 \0 저장
		temp_free = *buffer_backup;
		*buffer_backup = ft_strjoin(temp_free, *buffer);
		free_ptr(&temp_free);
	}
	return (bytes_read); // \n 들어가거나 \0 까지 온 경우 .... 맨 마지막에 읽은 값? => -1 이거나 0 인 경우 식별하기 위해서
}

static char	*get_line(int fd, char **buffer, char **buffer_backup)
{
	int		bytes_read;
	char	*temp_free;

	if (ft_strchr(*buffer_backup, '\n'))
		return (extract_line(buffer_backup));
	bytes_read = read_file(fd, buffer, buffer_backup); // buffer_backup에 \n 저장된경우, buffer 가 \0 까지 읽은 경우, read 에러인 경우
	if (!**buffer_backup || bytes_read == -1) //read 에러 || backup에 아무것도 저장 안된 경우
	// buffer_backup이 \0이면 bytes_read는 당연히 0 아님? 맞는거같음
	{
		free_ptr(buffer_backup);
		return (NULL);
	}
	if (ft_strchr(*buffer_backup, '\n')) // backup에 \n 있는 경우
		return (extract_line(buffer_backup));
	if (!ft_strchr(*buffer_backup, '\n') && **buffer_backup) // \0 까지 읽었는데 backup에 남아있는 경우
	{
		temp_free = ft_strdup(*buffer_backup);
		free_ptr(buffer_backup);
		return (temp_free);
	}
	return (NULL); //?
}

char	*get_next_line(int fd)
{
	static char		*buffer_backup[OPEN_MAX + 1];
	char			*buffer;
	char			*result;

	if (fd < 0 || fd > OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!buffer_backup[fd])
		buffer_backup[fd] = ft_strdup("");
	result = get_line(fd, &buffer, &buffer_backup[fd]);
	free_ptr(&buffer);
	return (result);
}
