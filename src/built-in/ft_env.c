/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:15:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/07 17:00:21 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_env *env)
{
	t_env	*temp;

	if (!env)
		return (1);
	temp = env;
	while (temp)
	{
		if (temp->key && temp->value)
			printf("%s%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	return (0);
}
