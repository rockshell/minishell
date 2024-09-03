/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:18:09 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/03 14:44:08 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_key(char *current_env)
{
	char	*key;
	size_t	i;

	i = 0;
	while (current_env[i] != '=')
		i++;
	i++;
	key = malloc(sizeof(char) * i + 1);
	if (!key)
	{
		ft_putstr_fd(ALLOC_ERROR, 2);
		return (NULL);
	}
	ft_strlcpy(key, current_env, i + 1);
	return (key);
}

static char	*get_value(char *current_env)
{
	int	i;

	i = 0;
	while (current_env[i] != '=')
		i++;
	i++;
	return (current_env + i);
}

int	create_node(t_env **env, char *current_env)
{
	t_env	*node;
	t_env	*last;

	last = *env;
	node = malloc(sizeof(t_env));
	if (!node)
		return (ft_putstr_fd(ALLOC_ERROR, 2), 1);
	node->key = get_key(current_env);
	if (!node->key)
		return (1);
	node->value = get_value(current_env);
	node->next = NULL;
	if (!*env)
	{
		*env = node;
		return (0);
	}
	while (last->next != NULL)
		last = last->next;
	last->next = node;
	return (0);
}
