/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_logic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/26 18:09:29 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/* 
reminder of the && operator :
if the left command succeeds, execute the right command
*/
int	handling_and(t_data *data, t_node *node, bool piped)
{
	int status;

	status = 0;
	status = handling_node(data, node->left, piped);
	if (status == 0)
		return (handling_node(data, node->right, piped));
	return (status);
}

/* 
reminder of the OR operator :
if the left command fails, execute the right command
*/
int	handling_or(t_data *data, t_node *node, bool piped)
{
	int status;

	status = 0;
	status = handling_node(data, node->left, piped);
	if (status != 0)
		return (handling_node(data, node->right, piped));
	return (status);
}
