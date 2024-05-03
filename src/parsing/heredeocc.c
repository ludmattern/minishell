/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredeocc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:05:51 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/02 18:20:18 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

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
		result = strdup(line);
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
		*total_size + strlen(line) + 1);
		if (!new_result)
			return (free(line), free(result), \
			perror("Realloc failed"), NULL);
		result = new_result;
		strcat(result, line);
	}
	*total_size += strlen(line);
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
			return (EXIT_GENERAL_ERROR);
	}
	if (result == NULL)
	{
		result = strdup("");
		if (!result)
			return (perror("Allocation failed"), EXIT_GENERAL_ERROR);
	}
	if (g_heredoc_sigint == 2)
		return (EXIT_FAILURE);
	*out_buffer = result;
	return (signals_init(), EXIT_SUCCESS);
}
