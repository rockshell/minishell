/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:44:59 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/22 18:01:40 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_token(int	i, t_token *current)
{
	current->pos = i;
	current->is_parsed = 0;
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
		return (input + i);
	}
	else if (*input == '\'')
	{
		i++;
		while (input[i] != '\'' && input[i])
			i++;
		if (input[i] == '\'')
			i++;
		return (input + i);
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

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
	tokens = NULL;
}
