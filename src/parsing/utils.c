/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   utils.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fprevot <fprevot@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/03/13 16:01:10 by fprevot		   #+#	#+#			 */
/*   Updated: 2024/03/13 16:08:16 by fprevot		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../inc/parse.h"

void	ft_strcpy(char *dest, const char *src)
{
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
}

void *my_realloc(void *ptr, size_t original_size, size_t new_size) 
{
    if (new_size == 0) 
	{
        free(ptr);
		ptr = NULL;
        return NULL;
    }
    void *new_ptr = malloc(new_size);
    if (!new_ptr) 
	{
        return NULL;
    }
    if (ptr) 
	{
        size_t size_to_copy = original_size < new_size ? original_size : new_size;
        ft_memcpy(new_ptr, ptr, size_to_copy);
        free(ptr);
		ptr = NULL;
    }

    return new_ptr;
}


char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	char	*ptr;

	ptr = dest + ft_strlen(dest);
	while (*src != '\0')
		*ptr++ = *src++;
	*ptr = '\0';
	return (dest);
}

void	imore(int k, int size, int *i)
{
	while (k++ < size)
	{
		(*i)++;
		k++;
	}
}
