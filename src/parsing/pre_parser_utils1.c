/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:46:08 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/09 18:27:15 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_token(int i, t_token *current)
{
	current->pos = i;
	current->is_parsed = 0;
	current->needs_expanding = 0;
	current->type = 0;
	current->prev = NULL;
	current->next = NULL;
}

int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

char	*handle_num_quotes(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '"')
	{
		i++;
		while (input[i] != '"' && input[i])
			i++;
		if (input[i] == '"')
			i++;
	}
	else if (*input == '\'')
	{
		i++;
		while (input[i] != '\'' && input[i])
			i++;
		if (input[i] == '\'')
			i++;
	}
	return (input + i);
}

size_t	handle_len_quotes(char *input, size_t i)
{
	if (input[i] == '"')
	{
		i++;
		while (input[i] != '"' && input[i])
			i++;
		if (input[i] == '"')
			i++;
		return (i);
	}
	else if (input[i] == '\'')
	{
		i++;
		while (input[i] != '\'' && input[i])
			i++;
		if (input[i] == '\'')
			i++;
		return (i);
	}
	return (0);
}

size_t	handle_len_redirs(char *input, size_t i)
{
	if (input[i] == '<')
	{
		if (input[i + 1] && input[i + 1] == '<')
			i++;
		i++;
	}
	else if (input[i] == '>')
	{
		if (input[i + 1] && input[i + 1] == '>')
			i++;
		i++;
	}
	return (i);
}

size_t	handle_len_pipes(char *input, size_t i)
{
	if (input[i + 1] && input[i + 1] == '|')
		i++;
	i++;
	return (i);
}