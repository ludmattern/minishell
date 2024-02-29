/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/02/29 09:46:14 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
* Main function of the shell. It will loop until the shell is closed.
*/
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;

  t_data *data = malloc(sizeof(t_data));
    if (data == NULL) {
        // Gestion d'erreur en cas d'échec d'allocation
        return 1;
    }
	ft_bzero(data, sizeof(t_data)); 
	data->ast = create_ast();
	//ex cmd is : cat < input.txt | grep "pattern" | sort > output.txt && echo "Success" || echo -n "Failure";
	//print_ast(data->ast, 0);
	run_execution(data);
	
	return (0);
}