/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:25:19 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/17 10:58:28 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Writes the arguments to the standard output followed by a newline (or not with 
option -n).
*/
int	ft_echo(char **args)
{
	int		i;
	bool	option;

	i = 1;
	option = false;
	if (args[1] && ft_strncmp(args[1], "-n", ft_strlen(args[1])) == 0)
	{
		option = true;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!option)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

/*
Check if the command is a forked builtin and execute it if it is
*/
// int	checking_non_forked_builtins(t_data *data, t_node *node)
// {
// 	int			status;
// 	const char	*str;

// 	str = (const char *)node->expanded_args[0];
// 	if (ft_strncmp(str, "/", ft_strlen(str) == 0))
// 		return (-1);
// 	else if (ft_strncmp(str, "cd", 2) == 0)
// 		status = ft_cd(node->expanded_args, data->env);
// 	else if (ft_strncmp(str, "export", 6) == 0)
// 		status = ft_export(node->expanded_args, &data->env);
// 	else if (ft_strncmp(str, "unset", 5) == 0)
// 		status = ft_unset_vars(node->expanded_args, &data->env);
// 	else
// 		return (-1);
// 	return(status);
// }

/*
Check if the command is a builtin and execute it if it is
*/
int	checking_forked_builtins(t_data *data, t_node *node)
{
	int			status;
	const char	*str;

	str = (const char *)node->expanded_args[0];
	if (ft_strncmp(str, "/", ft_strlen(str) == 0))
		return (0);
	else if (ft_strncmp(str, "echo", 5) == 0)
		status = ft_echo(node->expanded_args);
	else if (ft_strncmp(str, "pwd", 3) == 0)
		status = ft_pwd(node->expanded_args);
	else if (ft_strncmp(str, "env", 3) == 0)
		status = ft_env(node->expanded_args, &data->env);
	else
		return (0);
	//free_data_structure(&data);
	exit(status);
}

/*
Get the value of the environment variable.
*/
char	*ft_getenv(const char *name, char **env)
{
	size_t	name_len;
	size_t	i;

	name_len = ft_strlen(name);
	i = 0;
	while (env && env[i] != NULL)
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
			return (env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}
