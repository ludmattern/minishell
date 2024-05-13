/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:25:57 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/exec_bonus.h"
#include "../../inc/bonus/parse_bonus.h"

/*
Restores the original file descriptors.
*/
void	restore_original_fds(t_data *data)
{
	dup2(data->stdin, STDIN_FILENO);
	dup2(data->stdout, STDOUT_FILENO);
}

/*
Displays an error message if the fork fails.
*/
int	fork_creation_failure(const char *message)
{
	ft_eprintf(MS"%s: %s\n", message, strerror(errno));
	return (EXIT_FORK_FAILURE);
}

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
int	read_heredoc_and_write_to_pipe(const char *delimiter, int write_fd)
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
		if (write(write_fd, line, ft_strlen(line)) == -1)
		{
			perror("write failed");
			free(line);
			get_next_line(-1);
			return (EXIT_GENERAL_ERROR);
		}
		free(line);
	}
	get_next_line(-1);
	return (EXIT_SUCCESS);
}
