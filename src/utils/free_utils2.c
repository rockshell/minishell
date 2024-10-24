/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:59:54 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/24 16:12:02 by vitakinsfat      ###   ########.fr       */
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

void	error_rising(t_appdata *appdata)
{
	perror("Error");
	free_env(appdata->env);
	free_memory(appdata);
	exit(FAILURE);
}
