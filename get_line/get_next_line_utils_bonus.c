/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahilal <nahilal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:39:11 by nahilal           #+#    #+#             */
/*   Updated: 2024/12/17 13:39:11 by nahilal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	int		len;
	int		i;
	int		j;

	j = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *) malloc (len * sizeof(char));
	if (ptr == NULL)
		return (0);
	i = 0;
	while (s1[i])
		ptr[j++] = s1[i++];
	i = 0;
	while (s2[i])
		ptr[j++] = s2[i++];
	ptr[j] = '\0';
	return (ptr);
}

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*ptr;
	int		len;

	if (s1 == NULL)
		return (0);
	len = ft_strlen(s1)+ 1;
	ptr = (char *) malloc (len * sizeof(char));
	if (ptr == NULL)
		return (0);
	i = 0;
	while (s1[i] != '\0')
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	while (s[i] != (char)c)
	{
		if (s[i] == '\0')
			return (0);
		i++;
	}
	return ((char *)(s + i));
}

char	*ft_substr(char const *s, size_t start, size_t len)
{
	size_t	lenth;
	size_t	i;
	char	*ptr;

	i = 0;
	lenth = len + 1;
	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		lenth = ft_strlen(start + s) + 1;
	ptr = (char *) malloc (lenth * sizeof(char));
	if (ptr == NULL)
		return (0);
	while (i < len && s[start] != '\0')
		ptr[i++] = s[start++];
	ptr[i] = '\0';
	return (ptr);
}

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}
