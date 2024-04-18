/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 13:20:56 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/18 16:54:34 by lmattern         ###   ########.fr       */
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

t_token		*lex_me(char *in_put, int i);
t_node	*build_ast(t_token **current, int last_exit_status, t_g_data *g_data);
char	**expander(char *arg, int last_exit_status, t_g_data *data);
char		*get_command_path(char *cmd, t_g_data *g_data);
t_envsize get_mal_size(char *tkn, int start, int env_length, int i, t_env *mini_env, t_g_data *data);
char *get_env_var(char *tkn, int i, int k, int j, t_g_data *data);
char *get_env_var2(char *tkn, int i, int k, int j, t_g_data *data, bool dquotes);
char *skip_quote(char *tkn, char q, t_g_data *data);
char	**get_tkn_tab(char *arg, int size, int i, t_g_data *data);
char		*replace_substring(const char *original, \
	int start, const char *replace, t_g_data *data);
char		*ft_strncpy(char *dest, const char *src, size_t n);
void		imore(int size, int *i);
t_io_node *parse_io_from_command(char *cmd, int last_exit_status, t_g_data *data);
void		free_data_structure(t_data **data);
t_node *create_command_node(t_token *tkn, int last_exit_status, t_g_data *g_data);
t_node	*create_operator_node(t_token *tkn, t_g_data *g_data);
t_node	*create_empty_node(t_token *tkn, t_g_data *g_data);
void		printredir(t_io_node *io);
void		printlex(t_token *lexed);
void		print_ast(t_node *node, int depth);
void		print_exp(char **tab, char *arg);
bool		syntax_error(const char *cmd, int *status);
void		free_tree(t_node *node);
char	*ft_get_env3(char *tmp_env, t_env *mini_env, t_g_data *data, bool dquotes);
char	*ft_get_env2(char *tmp_env, t_env *mini_env, t_g_data *data);
void		free_mini_env(t_env *mini_env);
void	launch_expand(t_g_data *g_data);
void fail_exit_shell(t_g_data *g_data);
void	ft_clear_memory(t_g_data *g_data);
void expand_input(t_g_data *data);
char *replace_env_vars(t_g_data *data, int i);
void	print_start(void);
void	skip_spaces(char *arg, int *i);
void	free_forked_data(t_data *data);
char find_first(char *arg);
char	*expand_simple_quote(char *tkn, t_g_data *data);
char	*expand_without_quote(char *tkn, int last_exit_status, size_t i, t_g_data *data);
char	*expand_double_quote(char *tkn, int last_exit_status, t_g_data *data);
char *del_redir(char *cmd, int i, int j, t_g_data *g_data);

bool check_first(const char *cmd, char **token);
bool check_par(const char *cmd, char **token);
bool check_dquotes(const char *cmd);
bool check_squotes(const char *cmd);
bool check_redir(const char *cmd, char **token);
char **replace_input_vars(t_g_data *data, char *input, int i);
void add_new_io_node(t_io_bundle *io, char **cursor, int last_exit_status, t_g_data *data);
char	*extract_with_quote(char **c, t_g_data *data);
int countq(const char *str);
t_io_node *create_io_node_from_string(t_io_type type, char *value, int last_exit_status, t_g_data *data);
void set_io_type(t_io_type *type, char **cursor);

void	imore(int size, int *i);
void	imoremore_quote(char *arg, int *i, char c);
void	skip_space(char *input, int *index);
void	signals_init(void);
void	handle_sigint_heredoc(int sig);
void	handle_sigquit(int sig);
void	handle_sigint(int sig);
void	proc_handle_sigint(int sig);
void	proc_handle_sigquit(int sig);
void handle_sigint_herdoc(int sig);

bool	check_local(char *arg);
void	launch_parsing(t_g_data *g_data);
void	launch_lexing(t_g_data *g_data);
void	launch_execution(t_g_data *g_data);
void	update_history(t_g_data *g_data, int t);
void	update_input(t_g_data *g_data);
void	ft_clear_memory(t_g_data *g_data);
void	update_data(t_g_data *g_data);
void	free_parsing(t_node *ast, t_token *lex);
void	lex_mallox_error(t_token *lex);
void	free_lexed(t_token *lexed);
void	signals_ignore(void);


#endif