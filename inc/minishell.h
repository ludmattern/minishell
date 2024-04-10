/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:26:11 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/10 17:38:25 by fprevot          ###   ########.fr       */
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


typedef struct s_token
{
	t_token_type			type;
	struct s_token			*first;
	char					*value;
	char					**expanded;
	t_io_node				*io_list;
	struct s_token			*next;
	struct s_token			*prev;
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
	struct s_node		*left;
	struct s_node		*right;
}						t_node;


typedef struct s_data
{
	t_node			*ast;
	int				last_exit_status;
	int				stdin;
	int				stdout;
	char			**env;
	t_env			*mini_env;
	bool			signint_child;
	bool			heredoc_sigint;
	struct termios	initial_terminal_attributes;
}					t_data;


typedef struct s_global_data
{
	int				last_exit_status;
	int 			exit_fail;
	char			*in_put;
	char			*in_putsave;
	t_env			*mini_env;
	t_token			*save;
	t_token			*lexed;
	char			*path;
	char			*join;
	struct s_data	*data;
}					t_g_data;

#endif