/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:46:25 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/09 17:11:18 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Old version
// int	count_tokens(char *input)
// {
// 	int	i;

// 	i = 0;
// 	while (*input)
// 	{
// 		while (ft_isspace(*input) && *input)
// 			input++;
// 		if (*input == '\0')
// 			break ;
// 		if (!ft_isspace(*input))
// 		{
// 			i++;
// 			if (*input == '"' || *input == '\'')
// 				input = handle_num_quotes(input);
// 			else if (*input == '<' || *input == '>')
// 				input = handle_redirection_tokens(input);
// 			else
// 			{
// 				while (!ft_isspace(*input) && *input)
// 					input++;
// 			}
// 		}
// 	}
// 	return (i);
// }

//New version
int count_tokens(char *input)
{
	int	i;

	i = 0;
	while (*input)
	{
		while (ft_isspace(*input) && *input)
		{
			input++;
		}
		if (*input == '\0')
		{
			break;
		}
		while (!ft_isspace(*input) && *input)
		{
			// printf("kek\n");
			if (*input == '"' || *input == '\'')
			{
				input = handle_num_quotes(input);
				if (ft_isspace(*input) || *input == '\0')
					i++;
			}
			else if (*input == '<' || *input == '>')
			{
				input = handle_redirection_tokens(input);
				i++;
			}
			else
			{
				input++;
				if (ft_isspace(*input) || *input == '\0')
					i++;
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
			break ;
		}
		else
			i++;
	}
	return (i);
}

char	*handle_redirection_tokens(char *input)
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
	return (input + i);
}
