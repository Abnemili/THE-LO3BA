/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahilal <nahilal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:46:35 by nahilal           #+#    #+#             */
/*   Updated: 2024/12/21 17:25:48 by nahilal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_combine(int fd, char *ptr, char *save)
{
	ssize_t		nb;
	char		*tmp;

	nb = 1;
	while (nb > 0)
	{
		nb = read(fd, ptr, BUFFER_SIZE);
		if (nb == 0)
			break ;
		if (nb == -1)
			return (NULL);
		ptr[nb] = '\0';
		if (!save)
			save = ft_strdup("");
		tmp = save;
		save = ft_strjoin(tmp, ptr);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(ptr, '\n') != 0)
			break ;
	}
	return (save);
}

static char	*ft_new_str(char *line)
{
	int		len;
	char	*str;

	len = 0;
	while (line[len] != '\n' && line[len] != '\0')
		len++;
	if (line[len] == '\0' || line[1] == '\0')
		return (0);
	str = ft_substr(line, len + 1, ft_strlen(line) - len);
	if (str[0] == '\0')
	{
		free(str);
		str = NULL;
	}
	line[len + 1] = '\0';
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*save;
	char		*ptr;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	ptr = malloc (sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!ptr)
		return (NULL);
	line = ft_combine(fd, ptr, save);
	free(ptr);
	if (!line)
		return (NULL);
	save = ft_new_str(line);
	return (line);
}
