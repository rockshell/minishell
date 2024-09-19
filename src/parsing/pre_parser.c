/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:33:43 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/19 16:14:27 by arch             ###   ########.fr       */
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
		else
			i++;
	}
	return (i);
}

char	**fill_tokens(char *input, char **input_strings)
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
		len = len_of_input_string(input);
		input_strings[i] = malloc(sizeof(char) * (len + 1));
		if (!input_strings[i])
			return (free_tokens(input_strings), ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		ft_strlcpy(input_strings[i], token_start, len + 1);
		input = token_start + len;
		i++;
	}
	input_strings[i] = NULL;
	return (input_strings);
}

int	initial_parsing(char *input, t_appdata *appdata)
{
	int		token_num;

	token_num = count_tokens(input);
	appdata->num_of_input_strings = token_num;
	appdata->input_strings = malloc(sizeof(char *) * (token_num + 1));
	if (!appdata->input_strings)
		return (ft_putstr_fd(ALLOC_ERROR, 2), 1);
	appdata->input_strings = fill_tokens(input, appdata->input_strings);
	if (!appdata->input_strings)
		return (1);
	appdata->exec_data = malloc(sizeof(t_exec_data));
	if (!appdata->exec_data)
		return (1);
	return (0);
}

	// int i = 0;
	// while (appdata->tokens[i])
	// {
	// 	printf("Token %d is \"%s\"\n", i, appdata->tokens[i]);
	// 	i++;
	// }

