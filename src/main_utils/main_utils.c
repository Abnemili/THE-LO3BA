/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:15:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/19 20:33:58 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"
#include "../../parsing/parsing.h"

int	count_file_lines(int fd)
{
	char	*line;
	int		count;

	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	return (count);
}

char	**allocate_file_content(int line_count, int fd)
{
	char	**file_content;

	file_content = malloc(sizeof(char *) * (line_count + 1));
	if (!file_content)
	{
		close(fd);
		return (NULL);
	}
	return (file_content);
}

int	read_file_lines(char **file_content, int fd)
{
	char	*line;
	int		i;

	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		file_content[i] = line;
		i++;
		line = get_next_line(fd);
	}
	file_content[i] = NULL;
	return (i);
}

void	calculate_map_width(t_map *map, char **file_content, int height)
{
	int	j;
	int	len;

	map->width = 0;
	j = 0;
	while (j < height)
	{
		len = 0;
		while (file_content[j][len] && file_content[j][len] != '\n'
			&& file_content[j][len] != '\r')
			len++;
		if (len > map->width)
			map->width = len;
		j++;
	}
}
