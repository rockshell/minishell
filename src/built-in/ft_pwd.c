/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:14:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/08/30 15:16:46 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strncmp(temp->key, "PWD=", 4) == 0)
			printf("%s\n", temp->value);
		else
			temp = temp->next;
	}
}
