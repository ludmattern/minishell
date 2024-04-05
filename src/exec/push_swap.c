/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 19:32:34 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/04 19:33:21 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Duplicate the a stack into the secret stack to fort it without operating on it
*/
void	stack_cpy(t_stack_node **a, t_stack_node **secret_stack, size_t a_size)
{
	t_stack_node	*a_cursor;
	t_stack_node	*current;

	current = NULL;
	a_cursor = *a;
	while (a_size)
	{
		current = ft_lstnew(a_cursor->content);
		if (!current)
		{
			ft_lstclear(secret_stack);
			handle_error(a, NULL);
		}
		ft_lstadd_back(secret_stack, current);
		a_cursor = a_cursor->next;
		a_size--;
	}
}

/*
Sort the secret stack to obtain the right sorted index for the a stack numbers 
*/
void	sort_secret(t_stack_node **a, size_t stack_size, int swapped)
{
	t_stack_node	*current;
	t_stack_node	*next_node;
	long			temp;
	size_t			i;

	while (swapped)
	{
		swapped = 0;
		current = *a;
		i = -1;
		while (++i < stack_size - 1)
		{
			next_node = current->next;
			if (current->content > next_node->content)
			{
				temp = current->content;
				current->content = next_node->content;
				next_node->content = temp;
				swapped = 1;
			}
			current = current->next;
		}
		stack_size--;
	}
}

/*
Assign index to the secret_stack's numbers
*/
void	secret_index(t_stack_node **secret_stack, size_t a_size)
{
	t_stack_node	*current;
	size_t			i;

	i = 0;
	current = *secret_stack;
	while (i <= a_size - 1)
	{
		current->index = (int)i++;
		current = current->next;
	}
}

/*
Replace the a stack values by the secret stack numbers to performs the radix 
on it so there is no signed number to sort and minimum operations to performs
*/
void	replace_index(t_stack_node **a, t_stack_node **s_stack, size_t a_size)
{
	t_stack_node	*current_a;
	t_stack_node	*current_secret;
	size_t			i;

	current_a = *a;
	current_secret = *s_stack;
	i = 0;
	while (i < a_size)
	{
		current_secret = *s_stack;
		while (current_secret)
		{
			if (current_a->content == current_secret->content)
			{
				current_a->index = current_secret->index;
				break ;
			}
			current_secret = current_secret->next;
		}
		current_a = current_a->next;
		i++;
	}
}

/*
calls the mandatory function to sort the secret stack and replace a
stacks values accordingly to its sorted numbers positions
*/
void	secret_sort(t_stack_node **a, size_t a_size)
{
	t_stack_node	*secret_stack;

	secret_stack = NULL;
	stack_cpy(a, &secret_stack, a_size);
	sort_secret(&secret_stack, a_size, 1);
	secret_index(&secret_stack, a_size);
	replace_index(a, &secret_stack, a_size);
	ft_lstclear(&secret_stack);
}
