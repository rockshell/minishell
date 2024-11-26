/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:59:54 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/26 16:49:14 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_memory(t_appdata *appdata)
{
	int	i;

	if (!appdata)
		return ;
	if (appdata->lists && appdata->lists_num > 0)
	{
		i = -1;
		while (++i < appdata->lists_num)
			free_lists(&appdata->lists[i]);
		free(appdata->lists);
		appdata->lists_num = 0;
	}
	if (appdata->tokens && appdata->tokens_num > 0)
	{
		i = -1;
		while (++i < appdata->tokens_num)
		{
			if (appdata->tokens[i].value)
				free(appdata->tokens[i].value);
		}
		free(appdata->tokens);
		appdata->first_token = NULL;
	}
}

void	free_env(t_env *env)
{
	t_env	*temp;

	if (!env)
		return ;
	while (env)
	{
		temp = env;
		env = env->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
	env = NULL;
}

void	free_envp_array(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		if (envp[i])
			free(envp[i]);
		i++;
	}
	free(envp);
}
