/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:33:43 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/03 14:43:55 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			else
			{
				while (!ft_isspace(*input) && *input)
					input++;
			}
		}
	}
	return (i);
}

size_t	count_len_of_token(char *input)
{
	size_t	i;

	i = 0;
	while (ft_isspace(input[i]))
		input++;
	while (!ft_isspace(input[i]) && input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			i = handle_len_quotes(input, i);
		else
			i++;
	}
	return (i);
}

char	**fill_tokens(char *input, char **tokens)
{
	int		i;
	size_t	len;
	char	*token_start;

	i = 0;
	len = 0;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (*input == '\0')
			break ;
		token_start = input;
		len = count_len_of_token(input);
		tokens[i] = malloc(sizeof(char) * (len + 1));
		if (!tokens[i])
			return (free_tokens(tokens), ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		ft_strlcpy(tokens[i], token_start, len + 1);
		input = token_start + len;
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

int	initial_parsing(char *input, t_appdata *appdata)
{
	int		token_num;

	token_num = count_tokens(input);
	appdata->tokens = malloc(sizeof(char *) * (token_num + 1));
	if (!appdata->tokens)
		return (ft_putstr_fd(ALLOC_ERROR, 2), 1);
	appdata->tokens = fill_tokens(input, appdata->tokens);
	if (!appdata->tokens)
		return (1);
	return (0);
}

	// int i = 0;
	// while (appdata->tokens[i])
	// {
	// 	printf("Token %d is \"%s\"\n", i, appdata->tokens[i]);
	// 	i++;
	// }

