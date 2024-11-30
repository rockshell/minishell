/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:06:01 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/30 16:49:07 by arch             ###   ########.fr       */
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

