/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:55:16 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/25 16:10:13 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_service_tokens(t_appdata *appdata, char **input_strings)
{
	int i;
	int counter;

	i = -1;
	counter = 0;
	while (++i < appdata->num_of_input_strings)
	{
		if (get_type_of_token(input_strings[i]))
			counter++;
	}
	return (counter);
}

int count_command_tokens(t_appdata *appdata, char **input_strings)
{
	int i;
	int counter;

	i = -1;
	counter = 0;
	while (++i < appdata->num_of_input_strings)
	{
		if (!get_type_of_token(input_strings[i]))
		{
			counter++;
			while (!get_type_of_token(input_strings[i]))
				i++;
		}
	}
	return (counter);
}

int count_len_of_command_token(t_appdata *appdata, int i)
{
	int len;

	len = 0;
	while (get_type_of_token(appdata->input_strings[i]) == 0 && i < appdata->num_of_input_strings)
	{
		len++;
		i++;
	}
	return (len);
}

void fill_command_tokens(t_appdata *appdata)
{
	int i;
	int j;
	int pos;

	i = 0;
	j = 0;
	while (i < appdata->num_of_input_strings && get_type_of_token(appdata->input_strings[i]) == 0)
	{
		pos = 0;
		appdata->cmd_tokens[j].argc = count_len_of_command_token(appdata, i);
		if (appdata->cmd_tokens[j].argc > 0)
		{
			appdata->cmd_tokens[j].argv = malloc(sizeof(char *) * (appdata->cmd_tokens[j].argc + 1));
			if (!appdata->cmd_tokens[j].argv)
				error_rising(appdata);
			//while (get_type_of_token(appdata->input_strings[i]) == 0 && i < appdata->num_of_input_strings)
			while (pos < appdata->cmd_tokens[j].argc)
			{
				appdata->cmd_tokens[j].argv[pos] = ft_strdup(appdata->input_strings[i]);
				if (!appdata->cmd_tokens[j].argv[pos])
					error_rising(appdata);
				i++;
				pos++;
			}
			appdata->cmd_tokens[j].id = j;
			appdata->cmd_tokens[j].argv[pos] = NULL;
			j++;
		}
		while (get_type_of_token(appdata->input_strings[i]) != 0 && i < appdata->num_of_input_strings)
			i++;
	}
}

void fill_service_tokens(t_appdata *appdata)
{
	int i;
	int j;
	int type;

	i = -1;
	j = 0;
	type = 0;
	while (++i < appdata->num_of_input_strings)
	{
		type = get_type_of_token(appdata->input_strings[i]);
		if (type != 0)
		{
			appdata->srv_tokens[j].original = ft_strdup(appdata->input_strings[i]);
			appdata->srv_tokens[j].type = type;
			j++;
		}
	}
}

void run_lexer(t_appdata *appdata)
{
	appdata->srv_tokens_num = count_service_tokens(appdata, appdata->input_strings);
	appdata->cmd_tokens_num = count_command_tokens(appdata, appdata->input_strings);
	appdata->cmd_tokens = malloc(sizeof(t_cmd_token) * appdata->cmd_tokens_num);
	if (!appdata->cmd_tokens)
		return ;
	appdata->srv_tokens = malloc(sizeof(t_srv_token) * appdata->srv_tokens_num);
	if (!appdata->srv_tokens)
	{
		free(appdata->cmd_tokens);
		return ;
	}
	fill_service_tokens(appdata);
	fill_command_tokens(appdata);
}