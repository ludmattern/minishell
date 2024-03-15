/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printfunc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 13:30:18 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/14 16:16:20 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void printredir(t_io_node *io)
{
	while (io != NULL)
	{
		printf("\n==============IO=============\n");
		printf("evalue = %s type = %d\n", io->expanded_value[0], io->type);
		printf("==============IO=============\n");
		io = io->next;
	}
}


void print_ast(t_node* node, int depth) 
{

	if (node == NULL) 
		return;
	for (int i = 0; i < depth; i++)
		printf("  ");

	switch (node->type) 
	{
		case N_PIPE:
			printf("PIPE\n");
			break;
		case N_AND:
			printf("AND\n");
			break;
		case N_OR:
			printf("OR\n");
			break;
		case N_CMD:
			printf("CMD: %s\n", node->args); 
			break;
		default:
			printf("UNKNOWN\n");
			break;
	}
	if (node->left != NULL) 
	{
		for (int i = 0; i <= depth; i++) printf("  ");
		printf("-> Left:\n");
		print_ast(node->left, depth + 1);
	}
	if (node->right != NULL) 
	{
		for (int i = 0; i <= depth; i++) printf("  ");
		printf("-> Right:\n");
		print_ast(node->right, depth + 1);
	}
}

void printlex(t_token *lexed)
{
	for(;lexed != NULL; lexed = lexed->next)
		printf("%s->", lexed->value);
	printf("\n");
}

void print_exp(char **tab, char *arg)
{
	printf("BEFORE : %s\n", arg);
	int i = 0;
	while (tab[i])
	{
		printf("AFTER %i : %s\n", i, tab[i]);
		i++;
	}
	printf("\n\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
}