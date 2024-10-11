/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:18:09 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/11 14:51:18 by vitakinsfat      ###   ########.fr       */
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

	value = ft_strchr(current_env, '=');
	if (!value)
		return (NULL);
	return (ft_strdup(value + 1));
}

int	create_env_var_node(t_env **env, char *current_env)
{
	t_env	*node;
	t_env	*last;

	if (!env || !current_env)
		return (1);
	node = malloc(sizeof(t_env));
	if (!node)
		return (ft_putstr_fd(ALLOC_ERROR, 2), 1);
	node->key = get_key(current_env);
	if (!node->key)
		return (free(node), 1);
	node->value = get_value(current_env);
	node->next = NULL;
	if (!*env)
	{
		*env = node;
		return (0);
	}
	last = *env;
	while (last->next != NULL)
		last = last->next;
	last->next = node;
	return (0);
}

int	initialize_env_var(t_appdata *appdata, char **envp)
{
	int	i;

	i = 0;
	appdata->env_var = NULL;
	while (envp[i])
	{
		if (create_env_var_node(&appdata->env_var, envp[i]) == 1)
			return (1);
		i++;
	}
	return (0);
}
