/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yooshima <yooshima@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 12:57:58 by yooshima          #+#    #+#             */
/*   Updated: 2024/05/22 13:15:57 by yooshima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*make_buf(char **buf)
{
	*buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!*buf)
		return (NULL);
	return (*buf);
}

int	remain_read(int fd, char **remain, char **buf)
{
	int	read_byte;

	if (!*remain)
	{
		read_byte = read(fd, *buf, BUFFER_SIZE);
		if (read_byte == -1)
			return (-1);
		(*buf)[read_byte] = '\0';
		return (read_byte);
	}
	ft_memcpy(*buf, *remain, ft_strlen(*remain) + 1);
	free(*remain);
	*remain = NULL;
	return (1);
}

char	*check_newl(char **buf, char **remain, int *flag)
{
	int		i;
	char	*free_ptr;

	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			*flag = 1;
			if (ft_strlen(*(buf) + i + 1))
			{
				free_ptr = *remain;
				*remain = ft_strdup(*(buf) + i + 1);
				if (!*remain)
					return (free(*remain), NULL);
				free(free_ptr);
			}
			break ;
		}
		i++;
	}
	return (ft_substr(*buf, 0, i + 1));
}

char	*strjoin_free(char	**result, char *str)
{
	char	*free_ptr;
	char	*joined;

	if (!str)
		return (free(*result), NULL);
	free_ptr = *result;
	joined = ft_strjoin(*result, str);
	if (!joined)
		return (free(str), free(free_ptr), NULL);
	free(free_ptr);
	free(str);
	return (joined);
}

char	*get_next_line(int fd)
{
	static char	*remain = NULL;
	char		*buf;
	char		*result;
	int			flag;
	int			read_byte;

	read_byte = 1;
	flag = 0;
	if (fd < 0)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	if (!make_buf(&buf))
		return (free(result), NULL);
	while (!flag && read_byte)
	{
		read_byte = remain_read(fd, &remain, &buf);
		if (read_byte == -1 || (read_byte == 0 && !ft_strlen(result)))
			return (free(buf), free(result), NULL);
		result = strjoin_free(&result, check_newl(&buf, &remain, &flag));
		if (!result)
			return (free(buf), free(result), NULL);
	}
	return (free(buf), result);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q a.out");
// }

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <stdio.h>
// int main(void)
// {
// 	char *result;
// 	int fd = open("text.txt", O_RDONLY);
// 	for(int i = 0; i < 12; i++)
// 	{
// 		result = get_next_line(fd);
// 		printf("%s", result);
// 		if(!result)
// 			break;
// 		free(result);
// 	}
// 	free(result);
// 	close(fd);
// }