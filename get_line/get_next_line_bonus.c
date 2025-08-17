/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahilal <nahilal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:38:47 by nahilal           #+#    #+#             */
/*   Updated: 2024/12/17 13:38:47 by nahilal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_combine(int fd, char *ptr, char *save)
{
	int		nb;
	char	*tmp;

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

char	*ft_new_str(char *line)
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
	static char	*save[1024];
	char		*ptr;
	char		*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (0);
	ptr = malloc (sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!ptr)
		return (NULL);
	line = ft_combine(fd, ptr, save[fd]);
	free(ptr);
	ptr = NULL;
	if (!line)
		return (NULL);
	save[fd] = ft_new_str(line);
	return (line);
}
