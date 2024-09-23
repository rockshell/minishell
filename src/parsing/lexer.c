/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:55:16 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/23 18:47:11 by vitakinsfat      ###   ########.fr       */
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
		i++;
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
		else 
			i++;
	}
	return (counter);
}

void fill_argv(t_appdata *appdata, int i, int k)
{
	int len;
	int pos;

	len = 0;
	pos = 0; 
	while (get_type_of_token(appdata->input_strings[i]) == 0)
	{
		len++;
		i++;
	}
	appdata->cmd_tokens[k].argv = malloc(sizeof(char *) * (len + 1));
	i -= len;
	while (get_type_of_token(appdata->input_strings[i]) == 0)
	{
		appdata->cmd_tokens[k].argv[pos] = ft_strdup(appdata->input_strings[i]);
		pos++;
		i++;
	}
	appdata->cmd_tokens[k].argv[pos] = '\0';
}

void fill_tokens(t_appdata *appdata)
{
	int i;
	int j;
	int k;
	int type;

	i = -1;
	j = 0;
	k = 0;
	while (++i < appdata->num_of_input_strings)
	{
		type = get_type_of_token(appdata->input_strings[i]);
		if (type != 0)
		{
			appdata->srv_tokens[j].original = ft_strdup(appdata->input_strings[i]);
			appdata->srv_tokens[j].type = type;
			j++;
		}
		else
		{
			fill_argv(appdata, i, k);
			appdata->cmd_tokens[k].id = k;
			k++;
		}
		i++;
	}
}

void run_lexer(t_appdata *appdata)
{
	int	i;

	i = -1;
	appdata->srv_tokens_num = count_service_tokens(appdata, appdata->input_strings);
	appdata->cmd_tokens_num = count_command_tokens(appdata, appdata->input_strings);
	appdata->cmd_tokens = malloc(sizeof(t_cmd_token) * appdata->cmd_tokens_num);
	if (!appdata->cmd_tokens)
		return ;
	appdata->srv_tokens = malloc(sizeof(t_srv_token) * appdata->srv_tokens_num + 1);
	if (!appdata->srv_tokens)
	{
		free(appdata->cmd_tokens);
		return ;
	}
	fill_tokens(appdata);
}