/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:46:25 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/12/02 17:23:47 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


size_t	len_of_input_string(char *input)
{
	size_t	i;

	i = 0;
	while (input[i] && !ft_isspace(input[i]))
	{
		if (input[i] == '"' || input[i] == '\'')
			i = handle_len_quotes(input, i);
		else if (input[i] == '<' || input[i] == '>')
		{
			i = handle_len_redirs(input, i);
			break ;
		}
		else if (input[i] == '|')
		{
			i = handle_len_pipes(input, i);
			break ;
		}
		else
		{
			i++;
			if (ft_isspace(input[i]) || is_operator(input + i))
				break ;
		}
	}
	return (i);
}

char	*handle_redirection_tokens(char *input, int *counter)
{
	int	i;

	i = 0;
	if (input[i] == '<')
	{
		if (input[i + 1] && input[i + 1] == '<')
			i++;
		i++;
	}
	if (input[i] == '>')
	{
		if (input[i + 1] && input[i + 1] == '>')
			i++;
		i++;
	}
	(*counter)++;
	return (input + i);
}

char	*handle_pipe_tokens(char *input, int *counter)
{
	int	i;

	i = 0;
	if (input[i + 1] && input[i + 1] == '|')
		i++;
	i++;
	(*counter)++;
	return (input + i);
}

void	init_token(int i, t_token *current)
{
	current->pos = i;
	current->is_parsed = 0;
	current->type = 0;
	current->prev = NULL;
	current->next = NULL;
}

int	count_tokens(char *input)
{
	int	i;

	i = 0;
	while (*input)
	{
		while (ft_isspace(*input) && *input)
			input++;
		if (*input == '\0')
			break ;
		while (!ft_isspace(*input) && *input)
		{
			if (*input == '"' || *input == '\'')
			{
				input = handle_num_quotes(input);
				if (ft_isspace(*input) || *input == '\0')
					i++;
			}
			else if (*input == '<' || *input == '>')
				input = handle_redirection_tokens(input, &i);
			else if (*input == '|')
				input = handle_pipe_tokens(input, &i);
			else
			{
				input++;
				if (ft_isspace(*input) || *input == '\0' || is_operator(input))
					i++;
			}
		}
	}
	return (i);
}