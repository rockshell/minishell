/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:55:16 by akulikov          #+#    #+#             */
/*   Updated: 2024/10/09 17:10:49 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_cmd(t_appdata *appdata, int first, int last, t_list *list)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd) * (last - first + 1));
	set_pipes_in_cmd(appdata, &cmd, first, last);
	
}

void	make_a_list(t_appdata *appdata, int start, int end, int i)
{
	t_list	list;
	t_token	*cmd_start;
	t_token	*cmd_end;
	
	list = appdata->lists[i];
	list.size = 0;
	cmd_start = appdata->first_token;
	cmd_end = cmd_start;
	while (cmd_end->pos < end)
	{
		while (cmd_start->pos < start)
			cmd_start = cmd_start->next;
		while (is_cmd_end(cmd_end) == 0)
			cmd_end = cmd_end->next;
		make_cmd(appdata, cmd_start->pos, cmd_end->pos, &list);
	}
	
	
	
	
	
	
}

void  fill_the_lists(t_appdata *appdata, int num_of_lists)
{
	int	i;
	int	start_pos;
	int	end_pos;
	t_list	*lists;
	t_token	*current;
	
	i = 0;
	start_pos = 1;
	current = appdata->first_token;
	lists = appdata->lists;
	while (++i <= num_of_lists)
	{
		while (is_list_end(current) == 0)
		{
			end_pos = current->pos;
			current = current->next;
		}
		make_a_list(appdata, start_pos, end_pos, i);
	}
	
}

void run_lexer(t_appdata *appdata)
{
	int		num_of_lists;
	t_list	*lists;
	
	num_of_lists = count_lists(appdata);
	lists = malloc(sizeof(t_list) * num_of_lists);
	appdata->lists = lists;
	fill_the_lists(appdata, num_of_lists);
}








// int count_tokens(t_appdata *appdata, char **input_strings)
// {
// 	int i;
// 	int counter;

// 	i = -1;
// 	counter = 0;
// 	if (input_strings[0][0] != '\0')
// 		counter++;
// 	while (++i < appdata->num_of_input_strings)
// 	{
// 		if (get_type_of_string(input_strings[i]) == PIPE)
// 			counter++;
// 	}
// 	return (counter);
// }


// int count_argv_length(t_appdata *appdata, int i)
// {
// 	int len;

// 	len = 0;
// 	while (get_type_of_string(appdata->input_strings[i]) == WORD
// 			&& i < appdata->num_of_input_strings)
// 	{
// 		len++;
// 		i++;
// 	}
// 	return (len);
// }

// void fill_argv(t_appdata *appdata, int i, int j)
// {
// 	int k;

// 	k = 0;
// 	if (appdata->tokens[j].argc > 0)
// 	{
// 		appdata->tokens[j].argv = malloc(sizeof(char *) * (appdata->tokens[j].argc + 1));
// 		if (!appdata->tokens[j].argv)
// 			error_rising(appdata);
// 		while (k < appdata->tokens[j].argc)
// 		{
// 			appdata->tokens[j].argv[k] = ft_strdup(appdata->input_strings[i]);
// 			if (!appdata->tokens[j].argv[k])
// 				error_rising(appdata);
// 			i++;
// 			k++;
// 		}
// 		appdata->tokens[j].argv[k] = NULL;
// 	}
// }

// // void	fill_input_redirection(t_appdata *appdata, int i)
// // {
// // 	appdata->tokens->infile_name = ft_strdup(appdata->input_strings[i + 1]);
// // }

// void fill_tokens(t_appdata *appdata, char **input_strings, int num_of_input_strings)
// {
// 	int i;
// 	int j;

// 	i = 0;
// 	j = 0;
// 	while (i < num_of_input_strings)
// 	{
// 		if (get_type_of_string(input_strings[i]) == WORD)
// 		{
// 			appdata->tokens[j].argc = count_argv_length(appdata, i);
// 			fill_argv(appdata, i, j);
// 			i += appdata->tokens[j].argc;
// 		}
// 		else if (get_type_of_string(input_strings[i]) == IN_REDIR)
// 		{
// 			appdata->tokens->infile_name = ft_strdup(appdata->input_strings[i + 1]);
// 			// fill_input_redirection(appdata, i);
// 			i += 2;
// 		}	
// 		else if (get_type_of_string(input_strings[i]) == OUT_REDIR)
// 		{
// 			// fill_out_redirection(appdata, i);
// 			appdata->tokens->outfile_name = ft_strdup(appdata->input_strings[i + 1]);
// 			i += 2;
// 		}
// 		else if (get_type_of_string(input_strings[i]) == HERE_DOC_REDIR)
// 		{
// 			fill_here_doc_redirection(appdata, i);
// 			i += 2;
// 		}
// 		else if (get_type_of_string(input_strings[i]) == APPEND_REDIR)
// 		{
// 			fill_append_redirection(appdata, i);
// 			i += 2;
// 		}
// 		else if (get_type_of_string(input_strings[i]) == PIPE)
// 		{
// 			appdata->tokens[j].is_pipe_after = 1;
// 			if (j < appdata->tokens_num)
// 				appdata->tokens[j + 1].is_pipe_before = 1;
// 			i++;
// 		}
// 		j++;
// 	}
// }

// void run_lexer(t_appdata *appdata)
// {
// 	appdata->tokens_num = count_tokens(appdata, appdata->input_strings);
// 	appdata->tokens = malloc(sizeof(t_token) * appdata->tokens_num);
// 	if (!appdata->tokens)
// 		error_rising(appdata);
// 	fill_tokens(appdata, appdata->input_strings, appdata->num_of_input_strings);
// }

// // void fill_service_tokens(t_appdata *appdata)
// // {
// // 	int i;
// // 	int j;
// // 	int type;

// // 	i = -1;
// // 	j = 0;
// // 	type = 0;
// // 	while (++i < appdata->num_of_input_strings)
// // 	{
// // 		type = get_type_of_token(appdata->input_strings[i]);
// // 		if (type != 0)
// // 		{
// // 			appdata->srv_tokens[j].original = ft_strdup(appdata->input_strings[i]);
// // 			appdata->srv_tokens[j].type = type;
// // 			j++;
// // 		}
// // 	}
// // }

// // int count_command_tokens(t_appdata *appdata, char **input_strings)
// // {
// // 	int i;
// // 	int counter;

// // 	i = -1;
// // 	counter = 0;
// // 	while (++i < appdata->num_of_input_strings)
// // 	{
// // 		if (!get_type_of_token(input_strings[i]))
// // 		{
// // 			counter++;
// // 			while (!get_type_of_token(input_strings[i]))
// // 				i++;
// // 		}
// // 	}
// // 	return (counter);
// // }
