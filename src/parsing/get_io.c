/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:04:04 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/18 22:31:04 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

void	handle_sigint_herdoc(int sig)
{
	(void)sig;
	g_heredoc_sigint = 2;
	rl_done = 1;
	rl_redisplay();
}

void	setup_heredoc_si(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sigint_herdoc;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

int	read_heredoc_into_string(const char *delimiter, char **out_buffer)
{
	char	*line;
	char	*tmp;
	char	*result;
	size_t	total_size;
	char	*new_result;

	line = NULL;
	result = NULL;
	tmp = NULL;
	total_size = 0;
	setup_heredoc_si();
	while (g_heredoc_sigint == 0)
	{
		tmp = readline("> ");
		if (tmp == NULL || match_delimiter(tmp, delimiter))
		{
			free(line);
			break ;
		}
		line = ft_strjoin(tmp, "\n");
		free(tmp);
		tmp = NULL;
		if (result == NULL)
		{
			result = strdup(line);
			if (!result)
			{
				perror("Allocation failed");
				free(line);
				return (EXIT_GENERAL_ERROR);
			}
		}
		else
		{
			new_result = NULL;
			new_result = ft_realloc(result, total_size, \
			total_size + ft_strlen(line) + 1);
			if (!new_result)
			{
				perror("Reallocation failed");
				free(line);
				free(result);
				result = NULL;
				return (EXIT_GENERAL_ERROR);
			}
			result = new_result;
			ft_strcat(result, line);
		}
		total_size += ft_strlen(line);
		free(line);
	}
	if (result == NULL)
	{
		result = ft_strdup("");
		if (!result)
		{
			perror("Allocation failed");
			return (EXIT_GENERAL_ERROR);
		}
	}
	if (g_heredoc_sigint == 2)
	{
		return (EXIT_FAILURE);
	}
	signals_init();
	*out_buffer = result;
	return (EXIT_SUCCESS);
}

t_io_node	*create_io_node_from_string(t_io_type type, \
char *value, int last_exit_status, t_g_data *data)
{
	t_io_node	*io;

	io = malloc(sizeof(t_io_node));
	if (!io)
		fail_exit_shell(data);
	memset(io, 0, sizeof(t_io_node));
	io->type = type;
	if (io->type == IO_HEREDOC)
	{
		read_heredoc_into_string(value, &io->value);
		if (g_heredoc_sigint == 2)
			return (NULL);
	}
	else
		io->value = ft_strdup(value);
	if (!io->value)
		fail_exit_shell(data);
	if (io->type == IO_HEREDOC)
		io->expanded_value = replace_input_vars(data, io->value, 0);
	else
		io->expanded_value = expander(io->value, last_exit_status, data);
	io->here_doc = 0;
	io->prev = NULL;
	io->next = NULL;
	return (io);
}

t_io_node	*parse_io_from_command(char *cmd, int \
last_exit_status, t_g_data *data)
{
	t_io_bundle	io;

	io.head = NULL;
	io.tail = NULL;
	io.current_quote = 0;
	io.cursor = cmd;
	while (*io.cursor)
	{
		if ((*io.cursor == '"' || *io.cursor == '\'') && !io.current_quote)
			io.current_quote = *io.cursor;
		else if (*io.cursor == io.current_quote)
			io.current_quote = 0;
		else if ((*io.cursor == '>' || *io.cursor == '<') && !io.current_quote)
		{
			set_io_type(&io.type, &io.cursor);
			add_new_io_node(&io, &io.cursor, last_exit_status, data);
			if (g_heredoc_sigint == 2)
				return (NULL);
		}
		if (io.cursor >= cmd + ft_strlen(cmd))
			break ;
		io.cursor++;
	}
	return (io.head);
}
