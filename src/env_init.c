/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:18:09 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/03 15:08:19 by vitakinsfat      ###   ########.fr       */
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
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
	ft_strlcpy(key, current_env, i + 1);
	return (key);
}

static char	*get_value(char *current_env)
{
	int	i;
	char *value;

	i = 0;
	while (current_env[i] != '=')
		i++;
	i++;
	value = ft_strdup(current_env + i);
	if (!value)
		return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
	return (value);
}

int	create_env_var_node(t_env **env, char *current_env)
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
	if (!node->value)
		return (free(node->key), 1);
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

int initialize_env_var(t_appdata *appdata, char **envp)
{
	int i;

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
