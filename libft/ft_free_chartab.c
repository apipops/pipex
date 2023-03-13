/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_chartab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avast <avast@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:17:46 by avast             #+#    #+#             */
/*   Updated: 2023/03/08 12:05:39 by avast            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_chartab(char **tab, int index)
{
	int	i;

	i = 0;
	if (index < 0)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
	}
	else
	{
		while (index >= 0)
		{
			free(tab[index]);
			index--;
		}
	}
	free(tab);
}
