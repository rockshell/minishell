/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:31:12 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/11 14:52:33 by vitakinsfat      ###   ########.fr       */
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

	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
		return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
	temp = env;
	i = 0;
	while (temp)
	{
		result[i] = ft_strdup(temp->key);
		if (!result[i])
		{
			while (--i >= 0)
				free(result[i]);
			free(result);
			return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		}
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
			temp = array[i];
			array[i] = array[i + 1];
			array[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
	return (array);
}

int	is_valid_var(char *argument)
{
	int		i;
	char	*key;

	i = 1;
	key = get_key(argument);
	if (!key)
		return (-1);
	if (ft_isalpha(key[0]) != 1 && key[0] != '_')
		return (0);
	while (key[i] != '\0')
	{
		if (ft_isalnum(key[i]) == 1 || key[i] == '_')
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
