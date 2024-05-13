/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_codes.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:29:36 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:40:23 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_CODES_H
# define ERROR_CODES_H

/*
Success
*/
# define EXIT_SUCCESS 0

/*
General error codes
*/
# define EXIT_COMMAND_NOT_FOUND 127
# define EXIT_INVALID_COMMAND 126

/*
Specific error codes
*/
# define EXIT_GENERAL_ERROR 1
# define EXIT_SYNTAX_ERROR 2
# define EXIT_PERMISSION_DENIED 3
# define EXIT_EXEC_FAILURE 4
# define EXIT_FORK_FAILURE 5
# define EXIT_PIPE_FAILURE 6
# define EXIT_IS_A_DIRECTORY 7
# define EXIT_DUP2_FAILURE 8

#endif
