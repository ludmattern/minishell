/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_logic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/05 15:37:43 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/* 
reminder of the && operator :
if the left command succeeds, execute the right command
*/
int	handling_and(t_data *data, t_node *node)
{
	if (handling_node(data, node->left) == 0)
		return (handling_node(data, node->right));
	return (1);
}

/* 
reminder of the OR operator :
if the left command fails, execute the right command
*/
int	handling_or(t_data *data, t_node *node)
{
	if (handling_node(data, node->left) != 0)
		return (handling_node(data, node->right));
	return (0);
}
