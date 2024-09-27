/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:55:16 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/26 21:00:40 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_tokens(t_appdata *appdata, char **input_strings)
{
	int i;
	int counter;

	i = -1;
	counter = 0;
	if (input_strings[0][0] != '\0')
		counter++;
	while (++i < appdata->num_of_input_strings)
	{
		if (get_type_of_string(input_strings[i]) == PIPE)
			counter++;
	}
	return (counter);
}


int count_argv_length(t_appdata *appdata, int i)
{
	int len;

	len = 0;
	while (get_type_of_string(appdata->input_strings[i]) == WORD
			&& i < appdata->num_of_input_strings)
	{
		len++;
		i++;
	}
	return (len);
}

void fill_argv(t_appdata *appdata, int i, int j)
{
	int k;

	k = 0;
	if (appdata->tokens[j].argc > 0)
	{
		appdata->tokens[j].argv = malloc(sizeof(char *) * (appdata->tokens[j].argc + 1));
		if (!appdata->tokens[j].argv)
			error_rising(appdata);
		while (k < appdata->tokens[j].argc)
		{
			appdata->tokens[j].argv[k] = ft_strdup(appdata->input_strings[i]);
			if (!appdata->tokens[j].argv[k])
				error_rising(appdata);
			i++;
			k++;
		}
		appdata->tokens[j].argv[k] = NULL;
	}
}

// void	fill_input_redirection(t_appdata *appdata, int i)
// {
// 	appdata->tokens->infile_name = ft_strdup(appdata->input_strings[i + 1]);
// }

void fill_tokens(t_appdata *appdata, char **input_strings, int num_of_input_strings)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < num_of_input_strings)
	{
		if (get_type_of_string(input_strings[i]) == WORD)
		{
			appdata->tokens[j].argc = count_argv_length(appdata, i);
			fill_argv(appdata, i, j);
			i += appdata->tokens[j].argc;
		}
		else if (get_type_of_string(input_strings[i]) == IN_REDIR)
		{
			appdata->tokens->infile_name = ft_strdup(appdata->input_strings[i + 1]);
			// fill_input_redirection(appdata, i);
			i += 2;
		}	
		else if (get_type_of_string(input_strings[i]) == OUT_REDIR)
		{
			// fill_out_redirection(appdata, i);
			appdata->tokens->outfile_name = ft_strdup(appdata->input_strings[i + 1]);
			i += 2;
		}
		else if (get_type_of_string(input_strings[i]) == HERE_DOC_REDIR)
		{
			fill_here_doc_redirection(appdata, i);
			i += 2;
		}
		else if (get_type_of_string(input_strings[i]) == APPEND_REDIR)
		{
			fill_append_redirection(appdata, i);
			i += 2;
		}
		else if (get_type_of_string(input_strings[i]) == PIPE)
		{
			appdata->tokens[j].is_pipe_after = 1;
			if (j < appdata->tokens_num)
				appdata->tokens[j + 1].is_pipe_before = 1;
			i++;
		}
		j++;
	}
}

void run_lexer(t_appdata *appdata)
{
	appdata->tokens_num = count_tokens(appdata, appdata->input_strings);
	appdata->tokens = malloc(sizeof(t_token) * appdata->tokens_num);
	if (!appdata->tokens)
		error_rising(appdata);
	fill_tokens(appdata, appdata->input_strings, appdata->num_of_input_strings);
}

// void fill_service_tokens(t_appdata *appdata)
// {
// 	int i;
// 	int j;
// 	int type;

// 	i = -1;
// 	j = 0;
// 	type = 0;
// 	while (++i < appdata->num_of_input_strings)
// 	{
// 		type = get_type_of_token(appdata->input_strings[i]);
// 		if (type != 0)
// 		{
// 			appdata->srv_tokens[j].original = ft_strdup(appdata->input_strings[i]);
// 			appdata->srv_tokens[j].type = type;
// 			j++;
// 		}
// 	}
// }

// int count_command_tokens(t_appdata *appdata, char **input_strings)
// {
// 	int i;
// 	int counter;

// 	i = -1;
// 	counter = 0;
// 	while (++i < appdata->num_of_input_strings)
// 	{
// 		if (!get_type_of_token(input_strings[i]))
// 		{
// 			counter++;
// 			while (!get_type_of_token(input_strings[i]))
// 				i++;
// 		}
// 	}
// 	return (counter);
// }
