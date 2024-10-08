/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:31:12 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/08 18:23:38 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_length_of_env(t_env *env)
{
	int		counter;
	t_env	*temp;

	counter = 0;
	temp = env;
	while (temp)
	{
		counter++;
		temp = temp->next;
	}
	return (counter);
}

char	**get_an_array_of_keys(t_env *env, int len)
{
	t_env	*temp;
	char	**result;
	int		i;

	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (ft_putstr_fd(ALLOC_ERROR, 2), 1);
	temp = env;
	i = 0;
	while (temp)
	{
		result[i] = ft_strdup(temp->key);
		i++;
		temp = temp->next;
	}
	result[i] = NULL;
	return (result);
}

char	**sort_an_array(char **array, int len)
{
	char	*temp;
	int		i;

	i = 0;
	while (i < (len - 1))
	{
		if (ft_strcmp(array[i], array[i + 1]) > 0)
		{
			temp = ft_strdup(array[i]);
			free(array[i]);
			array[i] = ft_strdup(array[i + 1]);
			free(array[i + 1]);
			array[i + 1] = ft_strdup(temp);
			free(temp);
			i = 0;
		}
		else
			i++;
	}
	return (array);
}

int	is_valid_var(char *argument)
{
	int	i;

	i = 1;
	if (ft_isalpha(argument[0]) != 1 && argument[0] != '_')
		return (0);
	while (argument[i] != '\0')
	{
		if (ft_isalnum(argument[i]) == 1 || argument[i] == '_')
			i++;
		else
			return (0);
	}
	return (1);
}

int	is_in_var(t_env *env, char *argument)
{
	t_env	*temp;
	char	*key;

	temp = env;
	key = get_key(argument);
	if (!key)
		return (1);
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (1);
		else
			temp = temp->next;
	}
	free(key);
	return (0);
}
