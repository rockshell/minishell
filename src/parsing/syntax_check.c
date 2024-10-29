/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:44:13 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/29 16:46:11 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error_message(t_token *token)
{
	ft_putstr_fd(SYNTAX_ERROR, 2);
	ft_putstr_fd(token->value, 2);
	ft_putstr_fd("'\n", 2);
}

int	pipe_syntax_check(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp)
	{
		if (temp->type == PIPE)
		{
			if (temp->pos == 0 || temp->next == NULL
				|| (temp->next->type != WORD
					&& is_token_redirection(temp->next) == FALSE))
			{
				print_syntax_error_message(temp);
				return (FALSE);
			}
			else if (temp->prev != NULL && temp->prev->type != WORD)
			{
				print_syntax_error_message(temp);
				return (FALSE);
			}
		}
		temp = temp->next;
	}
	return (TRUE);
}

int	redirection_syntax_check(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp)
	{
		if (is_token_redirection(temp) == TRUE)
		{
			if (temp->next == NULL || temp->next->type != WORD)
			{
				print_syntax_error_message(temp);
				return (FALSE);
			}
		}
		temp = temp->next;
	}
	return (TRUE);
}

int	syntax_check(t_token *token)
{
	if (pipe_syntax_check(token) == FALSE)
		return (FALSE);
	else if (redirection_syntax_check(token) == FALSE)
		return (FALSE);
	return (TRUE);
}
