/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:06:01 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/06 23:47:43 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_cmd_end(t_token *token)
{
	if (token->type == PIPE)
		return (TRUE);
	if (token->type == LOGICAL_AND || token->type == LOGICAL_OR)
		return (TRUE);
	return (FALSE);
}

int	is_list_end(t_token *token)
{
	if (token->next == NULL)
		return (TRUE);
	if (token->type == LOGICAL_AND || token->type == LOGICAL_OR)
		return (TRUE);
	return (FALSE);
}

int	is_token_redirection(t_token *token)
{
	if (token->type >= STDIN && token->type <= APPEND)
		return (TRUE);
	return (FALSE);
}

int	count_lists(t_appdata *appdata)
{
	int		res;
	t_token	*current;

	res = 1;
	current = appdata->first_token;
	while (current)
	{
		if (current->type == LOGICAL_AND || current->type == LOGICAL_OR)
			res++;
		current = current->next;
	}
	return (res);
}

void	check_if_env(t_token *token)
{
	t_token	*temp;
	char	*check;
	int		i;

	i = 0;
	temp = token;
	while (temp)
	{
		check = ft_strchr(temp->value, '$');
		if (check && ft_isspace(check[i + 1]) == FALSE)
		{
			if (ft_strlen(temp->value) > 1)
				temp->needs_expanding = 1;
		}
		temp = temp->next;
	}
}
