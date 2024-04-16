/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_logic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/15 20:27:40 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/* 
reminder of the && operator :
if the left command succeeds, execute the right command
*/
int	handling_and(t_data *data, t_node *node, bool piped)
{
	if (handling_node(data, node->left, piped) == 0)
		return (handling_node(data, node->right, piped));
	return (1);
}

/* 
reminder of the OR operator :
if the left command fails, execute the right command
*/
int	handling_or(t_data *data, t_node *node, bool piped)
{
	if (handling_node(data, node->left, piped) != 0)
		return (handling_node(data, node->right, piped));
	return (0);
}
