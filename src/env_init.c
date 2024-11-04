/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:18:09 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/04 19:21:19 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_key(char *current_env)
{
	char	*key;
	size_t	len;

	len = 0;
	if (ft_strchr(current_env, '=') == NULL)
		len = ft_strlen(current_env);
	else
	{
		while (current_env[len] != '=')
			len++;
	}
	key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
	ft_strlcpy(key, current_env, len + 1);
	return (key);
}

char	*get_value(char *current_env)
{
	char	*value;

	if (ft_strchr(current_env, '=') != NULL)
	{
		value = ft_strchr(current_env, '=');
		if (!value)
			return (NULL);
		return (ft_strdup(value + 1));
	}
	return (NULL);
}

int	create_env_node(t_env **env, char *current_env)
{
	t_env	*node;
	t_env	*last;

	if (!env || !current_env)
		return (FAILURE);
	node = malloc(sizeof(t_env));
	if (!node)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	node->key = get_key(current_env);
	if (!node->key)
		return (free(node), FAILURE);
	node->value = NULL;
	if (ft_strchr(current_env, '=') != NULL)
	{
		node->value = get_value(current_env);
		if (!node->value)
			return(free(node->key), free(node), FAILURE);
	}
	node->next = NULL;
	if (!*env)
	{
		*env = node;
		return (SUCCESS);
	}
	last = *env;
	while (last->next != NULL)
		last = last->next;
	last->next = node;
	return (SUCCESS);
}

int	initialize_env_var(t_appdata *appdata, char **envp)
{
	int	i;

	i = 0;
	appdata->env = NULL;
	while (envp[i])
	{
		if (create_env_node(&appdata->env, envp[i]) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
