/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:33:43 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/09/30 19:04:26 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	count_tokens(char *input)
// {
// 	int	i;

// 	i = 0;
// 	while (input[i])
// 	{
		
// 	}
	
	// while (*input)
	// {
	// 	while (ft_isspace(*input) && *input)
	// 	{
	// 		input++;
	// 		// sleep(1);
	// 		// printf("whitespace\n");
	// 	}
	// 	if (*input == '\0')
	// 		break ;
	// 	if (!ft_isspace(*input))
	// 	{
	// 		i++;
	// 		if (*input == '"' || *input == '\'')
	// 			input = handle_num_quotes(input);
	// 		else
	// 		{
	// 			while (!ft_isspace(*input) && *input)
	// 				input++;
	// 		}
	// 	}
	// }
	// printf("Iterator is at %i\n", i);
// 	return (i);
// }

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

char	**fill_input_strings(char *input, char **input_strings)
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

int	is_operator(char *input, int i)
{
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
			return (LOGICAL_OR);
		else
			return (PIPE);
	}
	else if (input[i] == '<')
	{
		if (input[i + 1] == '<')
			return (HEREDOC);
		else
			return (STDIN);
	}
	else if (input[i] == '>')
	{
		if (input[i + 1] == '>')
			return (APPEND);
		else
			return(STDOUT);		
	}
	else if (input[i] == '&' && input[i + 1] == '&')
		return (LOGICAL_AND);
	return (0);
}

t_token		*handle_operator_token(char *input, int i)
{
	int	type;
	t_token	*token;
	
	type = is_operator(input, i);
	token->type = type;
	if (type == 2)
		token->value = ft_strdup("|");
	else if (type == 3)
		token->value = ft_strdup("<");
	else if (type == 4)
		token->value = ft_strdup(">");
	else if (type == 5)
		token->value = ft_strdup("<<");
	else if (type == 6)
		token->value = ft_strdup(">>");
	else if (type == 7)
		token->value = ft_strdup("&&");
	else if (type == 8)
		token->value = ft_strdup("||");
	return (token);
}

void	fill_tokens(char *input, t_appdata *appdata)
{
	int	i;
	int	j;
	int	pos;
	t_token *prev;
	t_token *current;

	i = 0;
	j = 0;
	pos = 1;
	prev = NULL;
	while (input[i] != '\0')
	{
		current = malloc(sizeof(t_token));
		current->pos = pos;
		if (is_operator(input, i) == WORD)
		{
			if (input[i] == '-')
				current->type = ARGUMENT;
			else
				current->type = WORD;
			while (is_operator(input, i) == WORD)
				j++;
			current->value = ft_substr(input, i, j - i);
			i = j + 1;	
		}
		else
		{
			current = handle_operator_token(input, i);
			i += ft_strlen(current->value);
		}
		if (prev != NULL)
		{
			current->prev = prev;
			prev->next = current;
		}
		else
			appdata->first_token = current;
		current = prev;
	}
}

int	run_parsing(char *input, t_appdata *appdata)
{
	int		token_num;
	
	fill_tokens(input, appdata);

	
	if (input_check(input))
		return (ft_putstr_fd(BAD_INPUT, 2), 1); 



	token_num = count_tokens(input);
	appdata->num_of_input_strings = token_num;
	appdata->input_strings = malloc(sizeof(char *) * (token_num + 1));
	if (!appdata->input_strings)
		return (ft_putstr_fd(ALLOC_ERROR, 2), 1);
	appdata->input_strings = fill_input_strings(input, appdata->input_strings);
	if (!appdata->input_strings)
		return (1);
	appdata->exec_data = malloc(sizeof(t_exec_data));
	if (!appdata->exec_data)
		return (1);
	return (0);
}
