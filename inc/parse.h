/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 13:20:56 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/20 13:20:32 by fprevot          ###   ########.fr       */
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

typedef enum e_token_type
{
	T_WORD,
	T_LESS,
	T_GREAT,
	T_DLESS,
	T_DGREAT,
	T_PIPE,
	T_AND,
	T_OR,
	T_NEWLINE,
	T_LPAR,
	T_RPAR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
	int		error;
}	t_token;

typedef struct s_envsize
{
	int		size;
	int		envsize;
	char	*env;
	char	*env_val;
	char	*res;
}		t_envsize;

typedef struct s_tkntab
{
	int		size;
	int		start;
	int		i;
}		t_tkntab;

t_token		*lex_me(char *in_put);
t_node		*build_ast(t_token **current, int last_exit_status);
char		**expander(char *arg, int last_exit_status);

char		*get_command_path(char *cmd);
char		*get_env_var(char *tkn, int i, int k, int j);
char		*skip_quote(char *tkn);
char	**get_tkn_tab(char *arg, int size, int i, int j);
char		*replace_substring(const char *original, \
	int start, int length, const char *replace);

void		ft_strcpy(char *dest, const char *src);
char		*ft_strncpy(char *dest, const char *src, size_t n);
void *my_realloc(void *ptr, size_t original_size, size_t new_size);
char		*ft_strcat(char	*dest, const char *src);
void		imore(int k, int size, int *i);

t_io_node	*make_io(t_token **tkn, int last_exit_status);

void		free_data_structure(t_data **data);

t_node		*create_command_node(t_token *tkn, int last_exit_status);
t_node		*create_operator_node(t_token *tkn);
t_node		*create_empty_node(t_token *tkn);
char		**init_tab(int capacity);

void		printredir(t_io_node *io);
void		printlex(t_token *lexed);
void		print_ast(t_node *node, int depth);
void		print_exp(char **tab, char *arg);

bool		check_syntax(const char *cmd);

void free_tree(t_node *node);

#endif