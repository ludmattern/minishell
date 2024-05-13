/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredeocc_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:05:51 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:24:32 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"
#include "../../inc/bonus/exec_bonus.h"

void	init_heredoc(size_t *total_size)
{
	setup_heredoc_si();
	*total_size = 0;
}

char	*read_and_append_newline(const char *delimiter)
{
	char	*tmp;
	char	*line;

	tmp = readline("> ");
	if (tmp == NULL || match_delimiter(tmp, delimiter))
	{
		free(tmp);
		return (NULL);
	}
	line = ft_strjoin(tmp, "\n");
	free(tmp);
	return (line);
}

char	*append_line_to_result(char *result, char *line, size_t *total_size)
{
	char	*new_result;

	if (result == NULL)
	{
		result = ft_strdup(line);
		if (!result)
		{
			perror("Allocation failed");
			free(line);
			return (NULL);
		}
	}
	else
	{
		new_result = ft_realloc(result, *total_size, \
		*total_size + ft_strlen(line) + 1);
		if (!new_result)
			return (free(line), free(result), \
			perror("Realloc failed"), NULL);
		result = new_result;
		ft_strcat(result, line);
	}
	*total_size += ft_strlen(line);
	free(line);
	return (result);
}

int	read_heredoc_into_string(const char *delimiter, char **out_buffer)
{
	char	*line;
	char	*result;
	size_t	total_size;

	init_heredoc(&total_size);
	result = NULL;
	while (g_heredoc_sigint == 0)
	{
		line = read_and_append_newline(delimiter);
		if (line == NULL)
			break ;
		result = append_line_to_result(result, line, &total_size);
		if (result == NULL)
			break ;
	}
	if (result == NULL)
	{
		result = ft_strdup("");
		if (!result)
			return (perror("Allocation failed"), EXIT_GENERAL_ERROR);
	}
	*out_buffer = result;
	if (g_heredoc_sigint == 2)
		return (EXIT_FAILURE);
	return (signals_init(), EXIT_SUCCESS);
}
