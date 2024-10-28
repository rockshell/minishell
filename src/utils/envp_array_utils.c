/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_array_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:31:13 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/25 18:29:08 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_free(t_appdata *appdata, int i)
{
	while (--i >= 0)
		free(appdata->envp[i]);
	free(appdata->envp);
}

static int	get_len_of_env(t_env *env)
{
	int		i;
	t_env	*temp;

	i = 0;
	temp = env;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

static int	renew_envp_array(t_appdata *appdata, t_env *env)
{
	char	*joined_str;
	t_env	*temp;
	int		i;

	i = 0;
	temp = env;
	while (temp)
	{
		joined_str = ft_strjoin(temp->key, "=");
		if (!joined_str)
			return (error_free(appdata, i), ft_putstr_fd(ALLOC_ERROR, 2), 1);
		appdata->envp[i] = ft_strjoin(joined_str, temp->value);
		free(joined_str);
		if (!appdata->envp[i])
			return (error_free(appdata, i), ft_putstr_fd(ALLOC_ERROR, 2), 1);
		i++;
		temp = temp->next;
	}
	appdata->envp[i] = NULL;
	return (SUCCESS);
}

int	update_envp_array(t_appdata *appdata, t_env *env)
{
	int	len;

	len = get_len_of_env(env);
	free_envp_array(appdata->envp);
	appdata->envp = malloc(sizeof(char *) * (len + 1));
	if (!appdata->envp)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	if (renew_envp_array(appdata, env) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	init_envp_array(t_appdata *appdata, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	appdata->envp = malloc(sizeof(char *) * (i + 1));
	if (!appdata->envp)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	i = 0;
	while (envp[i])
	{
		appdata->envp[i] = ft_strdup(envp[i]);
		if (!appdata->envp[i])
		{
			error_free(appdata, i);
			return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
		}
		i++;
	}
	appdata->envp[i] = NULL;
	return (SUCCESS);
}
