/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:15:19 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/10 14:44:53 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_node(t_env *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

void	unset_command(t_env **env, char *key)
{
	t_env	*temp;
	t_env	*prev;

	if (!env || !key)
		return ;
	temp = *env;
	if (temp && ft_strcmp(temp->key, key) == 0)
	{
		*env = temp->next;
		free_node(temp);
		return ;
	}
	while (temp && ft_strcmp(temp->key, key) != 0)
	{
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL)
		return ;
	prev->next = temp->next;
	free_node(temp);
}

int	ft_unset(t_cmd_token *token, t_env *env)
{
	int	i;

	i = 1;
	if (token->argc == 1)
		return (0);
	while (token->argv[i])
	{
		unset_command(&env, token->argv[i]);
		i++;
	}
	return (0);
}
