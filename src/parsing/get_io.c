/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:13:47 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/15 19:28:50 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

void signals_setup_heredoc(void)
{
    signal(SIGINT, handle_sigint_herdoc);
    signal(SIGQUIT, SIG_IGN);
}


void signals_restore(void) 
{
    signals_init(); 
}
void setup_heredoc_si(void) 
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint_herdoc;
    sa.sa_flags = 0; 
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}

int	read_heredoc_into_string(const char *delimiter, char **out_buffer)
{
	char	*line = NULL;
	char	*result = NULL;
	size_t	total_size = 0;

	setup_heredoc_si();
	while (g_heredoc_sigint == 0)
	{
		//line = get_next_line(STDIN_FILENO);
		line = readline("> ");
		if (line)
		{
			line = ft_strjoin(line, "\n");
		}
		if (line == NULL || match_delimiter(line, delimiter))
		{
			free(line);
			break;
		}
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
			char *new_result = NULL;
			new_result = ft_realloc(result, total_size, total_size + ft_strlen(line) + 1);
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
		//get_next_line(-1);
		return (EXIT_FAILURE);
	}
	signals_init();
	*out_buffer = result;
	//get_next_line(-1);
	return (EXIT_SUCCESS);
}

char **replace_input_vars(t_g_data *data, char *input)
{
	char **res;
    size_t i = 0;
    bool squotes = false;
	res = malloc(sizeof(char *) * 2);
    res[0] = ft_strdup(input);
	if (!res)
		fail_exit_shell(data);
    char *status_str;
	char *new;


    while (res[0] && res[0][i] != '\0') 
	{
        if (res[0][i] == '\'')
		{
            squotes = !squotes;
        } 
		else if (!squotes && res[0][i] == '$' && res[0][i + 1] == '?')
		{
            status_str = ft_itoa(data->last_exit_status);
            if (!status_str) fail_exit_shell(data); 
            new = replace_substring(res[0], i, 2, status_str, data);
            free(res[0]);
            res[0] = new;
			new = NULL;
            i += ft_strlen(status_str) - 1;
            free(status_str);
        }
		else if (!squotes && res[0][i] == '$' && res[0][i + 1] == '$')
            i += 1;
		else if (!squotes && res[0][i] == '$' && res[0][i + 1] != '\0' && res[0][i + 1] != ' ' && res[0][i + 1] != '"')
		{
            new = get_env_var(res[0], 0,0, 0, data);
            free(res[0]);
            res[0] = new;
			new = NULL;
        }
        i++;
    }
	res[1] = NULL;
    return (res);
}

t_io_node *create_io_node_from_string(t_io_type type, char *value, int last_exit_status, t_g_data *data) 
{
	t_io_node *io = malloc(sizeof(t_io_node));
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
		io->expanded_value = replace_input_vars(data, io->value);
	else
		io->expanded_value = expander(io->value, last_exit_status, data);
	io->here_doc = 0;
	io->prev = NULL;
	io->next = NULL;
	return (io);
}

static void set_io_type(t_io_type *type, char **cursor)
{
	if (**cursor == '>')
	{
		*type = IO_OUT;
		(*cursor)++;
		if (**cursor == '>')
		{
			*type = IO_APPEND;
			(*cursor)++;
		}
	}
	else if (**cursor == '<')
	{
		*type = IO_IN;
		(*cursor)++;
		if (**cursor == '<')
		{
			*type = IO_HEREDOC;
			(*cursor)++;
		}
	}
}

int countq(const char *str) 
{
	int count = 0;
	while (*str && *str != '>' && *str != '<' && *str != ' ') 
	{
		if (*str == '"') 
		{
			count++;
		}
		str++;
	}
	return count;
}

char *extract_with_quote(char **cursor,  t_g_data *data)
{
	char *result = malloc(strlen(*cursor) + 1); 
	if (!result)
		fail_exit_shell(data);
	char *output = result;
	int insideq = 0;

	while (**cursor && **cursor != '>' && **cursor != '<')
	{
		if (**cursor == '"' && !insideq) 
		{
			insideq = 1;
			(*cursor)++;
		}
		else if (**cursor == '"' && insideq) 
		{
			insideq = 0;
			(*cursor)++;
		}
		else 
		{
			*output++ = **cursor;
			(*cursor)++;
		}
		if (!insideq && **cursor == '"')
		{
			insideq = 1;
			(*cursor)++;
		}
	}
	*output = '\0';
	return (result);
}


void add_new_io_node(t_io_node **head, t_io_node **tail, \
							t_io_type type, char **cursor, int last_exit_status, t_g_data *data)
{
	char c = ' ';
	char *start;
	char *filename;
	
	while (**cursor == ' ')
		(*cursor)++;
	if (countq(*cursor) > 2 && **cursor == '"')
	{
		filename = extract_with_quote(cursor, data);
	}
	else
	{
		if (**cursor == '"')
		{
			c = '"';
			start = *cursor;
			(*cursor)++;
		}
		else
			start = *cursor;
		while (**cursor && **cursor != c && **cursor != '>' && **cursor != '<')
			(*cursor)++;
		filename = ft_strndup(start, *cursor - start); 
		if (!filename)
			fail_exit_shell(data);
	}
	t_io_node *new_io = create_io_node_from_string(type, filename, last_exit_status, data);
	if (g_heredoc_sigint == 2)
		return ;
	free(filename);
	if (!*head)
		*head = *tail = new_io;
	else
	{
		(*tail)->next = new_io;
		new_io->prev = *tail;
		*tail = new_io;
	}
}

t_io_node *parse_io_from_command(char *cmd, int last_exit_status, t_g_data *data)
{
	t_io_node *head = NULL;
	t_io_node *tail = NULL;
	char current_quote = 0;
	char *cursor = cmd;

	while (*cursor)
	{
		if ((*cursor == '"' || *cursor == '\'') && !current_quote)
			current_quote = *cursor;
		else if (*cursor == current_quote)
			current_quote = 0;
		else if ((*cursor == '>' || *cursor == '<') && !current_quote)
		{
			t_io_type type;
			set_io_type(&type, &cursor);
			add_new_io_node(&head, &tail, type, &cursor, last_exit_status, data);
			if (g_heredoc_sigint == 2)
				return (NULL);
		}
		if (cursor >= cmd + ft_strlen(cmd)) 
			break ;	
		cursor++;
	}
	
	return (head);
}
