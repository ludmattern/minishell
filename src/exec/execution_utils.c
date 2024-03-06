/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/06 18:23:04 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	command_exec_failure(const char *error) __attribute__((noreturn));
void	execute_command(t_data *data, t_node *node) __attribute__((noreturn));

/*
Waits for the child process to finish and return its exit status.
*/
int	wait_for_child(pid_t pid, t_data *data)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->last_exit_status = 128 + WTERMSIG(status);
	else
		data->last_exit_status = EXIT_FAILURE;
	return (data->last_exit_status);
}

/*
Returns true if the line matches the delimiter, false otherwise.
*/
bool	match_delimiter(const char *line, const char *delimiter)
{
	size_t	line_len;
	size_t	delimiter_len;

	line_len = ft_strlen(line);
	delimiter_len = ft_strlen(delimiter);
	if (line_len > 0 && line[line_len - 1] == '\n')
		line_len--;
	if (line_len == delimiter_len && ft_strncmp(line, delimiter,
			delimiter_len) == 0)
		return (true);
	return (false);
}

/*
Reads the heredoc and writes it to the pipe.
*/
void	read_heredoc_and_write_to_pipe(const char *delimiter, int write_fd)
{
	char	*line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL || match_delimiter(line, delimiter))
		{
			free(line);
			break ;
		}
		write(write_fd, line, strlen(line));
		free(line);
	}
	get_next_line(-1);
}
