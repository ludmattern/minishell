/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:14:40 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 18:19:28 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"
# include "./error_codes.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <linux/limits.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>

# define MS "minishell: "

extern int	g_heredoc_sigint;

typedef enum e_ast_direction
{
	AST_LEFT,
	AST_RIGHT
}	t_ast_direction;

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

typedef enum e_io_type
{
	IO_IN,
	IO_OUT,
	IO_HEREDOC,
	IO_APPEND
}	t_io_type;

typedef struct s_io_node
{
	t_io_type			type;
	char				*value;
	char				**expanded_value;
	int					here_doc;
	struct s_io_node	*prev;
	struct s_io_node	*next;
}						t_io_node;

typedef struct s_io_bundle
{
	t_io_type	type;
	t_io_node	*head;
	t_io_node	*tail;
	char		current_quote;
	char		*cursor;
}				t_io_bundle;
typedef struct s_token
{
	t_token_type			type;
	struct s_token			*first;
	char					*value;
	char					**expanded;
	bool					is_add_local;
	bool					is_empty;
	t_io_node				*io_list;
	struct s_token			*next;
	struct s_token			*prev;
	struct s_token			*last;
	struct s_global_data	*g_data;
	int						error;
}							t_token;

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
}			t_tkntab;

typedef enum e_node_type
{
	N_PIPE,
	N_AND,
	N_OR,
	N_CMD,
	N_INIT
}	t_node_type;

typedef struct s_path
{
	char	**paths;
	char	*full_cmd_path;
	char	*cmd_path;
	char	*path_env;
	char	*temp;
}					t_path;

typedef struct s_env
{
	char			*name;
	char			*value;
	bool			is_local;
	struct s_env	*next;
}					t_env;

typedef struct s_init_vars
{
	char	cwd[1024];
	int		shell_lvl;
	char	*shell_lvl_str;
	char	*shell_lvl_cmd;
	char	*tmp;
	char	*pwd_cmd;
}				t_init_vars;

typedef struct s_node
{
	t_node_type			type;
	t_io_node			*io_list;
	char				*args;
	char				*command_path;
	char				**expanded_args;
	bool				is_add_local;
	bool				is_empty;
	struct s_node		*left;
	struct s_node		*right;
}						t_node;

typedef struct s_data
{
	t_node					*ast;
	int						last_exit_status;
	int						stdin;
	int						stdout;
	char					**env;
	t_env					*mini_env;
	bool					signint_child;
	struct termios			initial_terminal_attributes;
	struct s_global_data	*g_data;
}					t_data;

typedef struct s_env_context
{
	char					*res;
	char					*status_str;
	char					*new;
	size_t					*i;
	bool					dquotes;
	bool					squotes;
	struct s_global_data	*data;
}			t_env_context;

typedef struct s_env_context_her
{
	char					**res;
	char					*status_str;
	char					*new;
	size_t					*i;
	bool					dquotes;
	bool					squotes;
	struct s_global_data	*data;
}			t_env_context_her;

typedef struct s_global_data
{
	int				last_exit_status;
	int				exit_fail;
	char			*in_put;
	char			*in_putsave;
	t_env			*mini_env;
	t_token			*save;
	t_token			*lexed;
	char			*path;
	char			*join;
	char			*pre_input;
	struct s_data	*data;
}					t_g_data;

typedef struct s_var_context
{
	int			i;
	int			k;
	int			j;
	int			start;
	char		*tmp_env;
	char		*res;
	int			env_length;
	char		*env_val;
	t_envsize	s;
}	t_var_context;

typedef struct s_var_size_context
{
	int			start;
	int			env_length;
	int			i;
	char		*env_val;
	t_envsize	s;
}	t_var_size_context;

#endif