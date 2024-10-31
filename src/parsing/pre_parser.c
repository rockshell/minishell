/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:33:43 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/31 17:07:14 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '|')
	{
		if (input[i + 1] && input[i + 1] == '|')
			return (LOGICAL_OR);
		else
			return (PIPE);
	}
	else if (input[i] == '<')
	{
		if (input[i + 1] && input[i + 1] == '<')
			return (HEREDOC);
		else
			return (STDIN);
	}
	else if (input[i] == '>')
	{
		if (input[i + 1] && input[i + 1] == '>')
			return (APPEND);
		else
			return (STDOUT);
	}
	else if (input[i] == '&' && input[i + 1] && input[i + 1] == '&')
		return (LOGICAL_AND);
	return (WORD);
}

char *handle_redirection_tokens(char *input)
{
	int	i;

	i = 0;
	if (input[i] == '<')
	{
		if (input[i+1] && input[i+1] == '<')
			i++;
		i++;
		// printf("Tokens num: %i\n", (*tokens_num));
		// (*tokens_num)++;
		// printf("Tokens num: %i\n", (*tokens_num));
	}
	if (input[i] == '>')
	{
		if (input[i+1] && input[i+1] == '>')
			i++;
		i++;
		// printf("Tokens num: %i\n", (*tokens_num));
		// (*tokens_num)++;
		// printf("Tokens num: %i\n", (*tokens_num));
	}
	return (input + i);
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
		if (!ft_isspace(*input))
		{
			i++;
			if (*input == '"' || *input == '\'')
				input = handle_num_quotes(input);
			else if (*input == '<' || *input == '>')
				// input = handle_redirection_tokens(input, &i);
				input = handle_redirection_tokens(input);
			else
			{
				while (!ft_isspace(*input) && *input)
					input++;
			}
		}
	}
	return (i);
}

size_t	len_of_input_string(char *input)
{
	size_t	i;

	i = 0;
	while (ft_isspace(input[i]))
		input++;
	while (!ft_isspace(input[i]) && input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			i = handle_len_quotes(input, i);
		else if (input[i] == '<' || input[i] == '>')
		{
			i = handle_len_redirs(input, i);
			break;
		}
		else
			i++;
	}
	return (i);
}

void	get_to_the_token(char *input, int *i)
{
	while (ft_isspace(input[*i]))
		(*i)++;
}

//TODO - expand expandable tokens
void	make_token(char *input, int *start, int token_pos, t_token *current)
{
	size_t	len;
	char	*value;

	init_token(token_pos, current);
	len = len_of_input_string(input + *start);
	value = malloc(sizeof(char) * (len + 1));
	get_to_the_token(input, start);
	ft_strlcpy(value, input + *start, len + 1);
	current->value = ft_strtrim(value, " ");
	current->type = is_operator(value);
	free(value);
	*start += (int)len;
}

int	run_parsing(char *input, t_appdata *appdata)
{
	int		i;
	int		j;
	t_token	*prev;

	i = -1;
	j = 0;
	prev = NULL;
	appdata->tokens_num = count_tokens(input);
	appdata->tokens = malloc(sizeof(t_token) * appdata->tokens_num);
	while (++i < appdata->tokens_num)
	{
		make_token(input, &j, i, &appdata->tokens[i]);
		if (prev != NULL)
		{
			appdata->tokens[i].prev = prev;
			prev->next = &appdata->tokens[i];
		}
		else
			appdata->first_token = &appdata->tokens[i];
		prev = &appdata->tokens[i];
	}
	return (0);
}
