/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:33:43 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/02 17:57:55 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_tokens(char *input)
{
	int i = 0;

	while (*input)
	{
		if (!ft_isspace(*input))
		{
			i++;
			if (*input == '"')
			{
				input++;
				while (*input != '"' && *input)
					input++;
				if (*input == '"')
					input++;
			}
			else if (*input == '\'')
			{
				input++;
				while (*input != '\'' && *input)
					input++;
				if (*input == '\'')
					input++;
			}
			else
			{
				while (!ft_isspace(*input) && *input)
					input++;
			}
		}
		else if (ft_isspace(*input))
		{
			if (is_space_str(input))
				return (i);
			i++;
			while (ft_isspace(*input) && *input)
				input++;
		}
	}
	return (i);
}

int count_len_of_token(char *input, int space)
{
	int i = 0;
	
	if (space == 1)
	{
		i++;
		return (i);
	}
	while (!ft_isspace(input[i]) && input[i])
	{
		if (input[i] == '"')
		{
			i++;
			while (input[i] != '"' && input[i])
				i++;
			if (input[i] == '"')
				i++;
		}
		else if (input[i] == '\'')
		{
			i++;
			while (input[i] != '\'' && input[i])
				i++;
			if (input[i] == '\'')
				i++;
		}
		else
			i++;
	}
	return (i);
}

char **fill_tokens(char *input, char **tokens)
{
	int i = 0;
	int j;
	int len = 0;

	while (*input)
	{
		len = count_len_of_token(input, ft_isspace(*input));
		tokens[i] = malloc(sizeof(char) * (len + 1));
		if (!tokens[i])
			return (NULL);
		j = 0;
		while (*input && j < len)
		{
			if (ft_isspace(*input))
			{
				tokens[i][j++] = *input++;
				while (ft_isspace(*input))
					input++;
			}
			else
				tokens[i][j++] = *input++;
		}
		tokens[i][j] = '\0';
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

int	initial_parsing(char *input, t_appdata *appdata)
{
	char **tokens;
	int token_num;
	int i = 0;
	
	(void)appdata;
	printf("%s\n", input);
	token_num = count_tokens(input);
	printf("%d\n", token_num);
	tokens = malloc(sizeof(char *) * (token_num + 1));
	if (!tokens)
		return (1);
	tokens = fill_tokens(input, tokens);
	tokens[token_num + 1] = NULL;
	while (tokens[i])
	{
		printf("Token %d is \"%s\"\n", i, tokens[i]);
		i++;
	}
	return (0);
}