/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 23:59:33 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/30 18:48:05 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_expandables(char *value)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (value[i])
	{
		if (value[i] == '$')
			counter++;
		i++;
	}
	return (counter);
}

int	no_sep(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (ft_isspace(value[i]) == TRUE)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

char	*expand_env_var(char *key, t_env *env, t_env *exit_status)
{
	char	*env_var;

	if (ft_strcmp(key, "?") == 0)
	{
		env_var = ft_get_env(exit_status, key);
		if (!env_var)
			return (NULL);
	}
	else
	{
		env_var = ft_get_env(env, key);
		if (!env_var)
			return (NULL);
	}
	return (env_var);
}
