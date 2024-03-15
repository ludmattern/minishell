/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 13:20:56 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/14 16:34:06 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "libft/inc/libft.h"
# include <termios.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
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
}	t_token;

typedef enum e_node_type
{
	N_PIPE,
	N_AND,
	N_OR,
	N_CMD,
	N_EMPTY
}		t_node_type;

typedef enum e_io_type
{
	IO_IN,
	IO_OUT,
	IO_HEREDOC,
	IO_APPEND
}		t_io_type;

typedef struct s_io_node
{
	t_io_type			type;
	char				*value;
	char				**expanded_value;
	int					here_doc;
	struct s_io_node	*prev;
	struct s_io_node	*next;
}		t_io_node;

typedef struct s_node
{
	t_node_type		type;
	t_io_node		*io_list;
	char			*args;
	char			*command_path;
	char			**expanded_args;
	struct s_node	*left;
	struct s_node	*right;
}		t_node;

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

typedef struct s_data
{
	t_node			*ast;
	int				last_exit_status;
	int				stdin;
	int				stdout;
	char			**env;
	bool			signint_child;
	bool			heredoc_sigint;
	struct termios	initial_terminal_attributes;
}	t_data;

t_token		*lex_me(char *in_put);
t_node		*build_ast(t_token **current, int last_exit_status);
char		**expander(char *arg, int last_exit_status);

char		*get_command_path(char *cmd);
char		*get_env_var(char *tkn, int i, int k, int j);
char		*skip_quote(const char *tkn);
char		**get_tkn_tab(char *arg, int size, int i, int k);
char		*replace_substring(const char *original, \
	int start, int length, const char *replace);

void		ft_strcpy(char *dest, const char *src);
char		*ft_strncpy(char *dest, const char *src, size_t n);
void		*my_realloc(void *ptr, size_t new_size);
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

#endif