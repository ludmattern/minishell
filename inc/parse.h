/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 13:20:56 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/04 13:05:44 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "../libft/inc/libft.h"
# include "./minishell.h"
# include <termios.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <limits.h>

t_token		*lex_me(char *in_put);
t_node		*build_ast(t_token **current, int last_exit_status);
char	**expander(char *arg, int last_exit_status, t_global_data *data);
char		*get_command_path(char *cmd);
char *get_env_var(char *tkn, int i, int k, int j, t_global_data *data);
char		*skip_quote(char *tkn, char q);
char		**get_tkn_tab(char *arg, int size, int i, int j);
char		*replace_substring(const char *original, \
	int start, int length, const char *replace);
char		*ft_strncpy(char *dest, const char *src, size_t n);
void		imore(int size, int *i);
t_io_node *parse_io_from_command(char *cmd, int last_exit_status, t_global_data *data);
void		free_data_structure(t_data **data);
t_node		*create_command_node(t_token *tkn, int last_exit_status);
t_node		*create_operator_node(t_token *tkn);
t_node		*create_empty_node(t_token *tkn);
char		**init_tab(int capacity);
void		printredir(t_io_node *io);
void		printlex(t_token *lexed);
void		print_ast(t_node *node, int depth);
void		print_exp(char **tab, char *arg);
bool		syntax_error(const char *cmd, int *status);
void		free_tree(t_node *node);

#endif