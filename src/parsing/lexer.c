/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:55:16 by akulikov          #+#    #+#             */
/*   Updated: 2024/10/18 16:05:11 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	make_cmd(t_appdata *appdata, int first, int last, int is_pipe_before_flag)
void init_cmd(t_cmd *cmd, t_token *first_token, t_token *last_token, int is_pipe_before_flag)
{
	// t_cmd	cmd;

	// cmd = malloc(sizeof(t_cmd));
	cmd->argc = 0;
	cmd->input_redir_type = 0;
	cmd->output_redir_type = 0;
	cmd->is_pipe_before = 0;
	cmd->is_pipe_after = 0;
	cmd->argv = NULL;
	cmd->infile_name = NULL;
	cmd->outfile_name = NULL;
	cmd->delim = NULL;
	// printf("Making a new command\n");
	set_pipes_in_cmd(cmd, is_pipe_before_flag, last_token);
	// printf("Pipes are in place\n");
	set_redirections_in_cmd(cmd, first_token);
	// printf("Redirections are in place\n");
	set_the_command_itself(cmd, first_token);
	// printf("The command itself ready\n");
	// return(cmd);
	printf("Argc: %i\n", cmd->argc);
	printf("Input redirection: %i\n", cmd->input_redir_type);
	printf("Output redirection: %i\n", cmd->output_redir_type);
	printf("Pipe before: %i\n", cmd->is_pipe_before);
	printf("Pipe after: %i\n", cmd->is_pipe_after);
	printf("Argv[0]: %s\n", cmd->argv[0]);
}


t_list	init_the_list(int start, int end)
{
	t_list	list;

	list.size = 0;
	list.and_after = 0;
	list.or_after = 0;
	list.end_after = 0;
	list.cmd = malloc(sizeof(t_cmd *) * (end - start)); //TODO - deal with overallocate 
	list.exec_data = malloc(sizeof(t_exec_data));
	return (list);
}

t_list	make_a_list(t_appdata *appdata, int start, int end)
{
	int	j;
	int	is_pipe_before_flag;
	// t_cmd	cmd;
	t_list	list;
	t_token	*cmd_start;
	t_token	*cmd_end;
	
	j = 0;
	is_pipe_before_flag = 0;
	cmd_start = &appdata->tokens[start];
	// printf("Value of the first token: %s\n", cmd_start->value);
	cmd_end = cmd_start;
	list = init_the_list(start, end);
	while (cmd_end && cmd_end->pos <= end)
	{
		while (is_cmd_end(cmd_end) == 0 && cmd_end->next != NULL)
			cmd_end = cmd_end->next;
		// printf("kek\n");
		// cmd = make_cmd(appdata, cmd_start->pos, cmd_end->pos, is_pipe_before_flag);
		// printf("kek\n");
		init_cmd(&list.cmd[j++], cmd_start, cmd_end, is_pipe_before_flag);
		if (cmd_end->type == 2)
			is_pipe_before_flag = 1;
		// list.cmd[j++] = cmd;
		list.size++;
		if (cmd_end->next)
		{
			cmd_start = cmd_end->next;
			cmd_end = cmd_start;
		}
		else
			break;
	}
	if (appdata->tokens[end].type == LOGICAL_AND)
		list.and_after = 1;
	else if(appdata->tokens[end].type == LOGICAL_OR)
		list.or_after = 1;
	else
		list.end_after = 1;
	// printf("\nargc = %d\n", list.cmd[j-1].argc);
	// printf("input_redir_type = %d\n", list.cmd[j-1].input_redir_type);
	// printf("output_redir_type = %d\n", list.cmd[j-1].output_redir_type);
	// printf("is_pipe_after = %d\n", list.cmd[j-1].is_pipe_after);
	// printf("infile name = %s\n", list.cmd[j-1].infile_name);
	// printf("outfile name = %s\n", list.cmd[j-1].outfile_name);
	// printf("argv[0] = %s\n", list.cmd[j-1].argv[0]);
	return (list);
}


void run_lexer(t_appdata *appdata)
{
	int		i;
	int		num_of_lists;
	int		start_pos;
	int		end_pos;
	t_token	*current;
	t_list	*lists;
	
	i = -1;
	start_pos = 0;
	num_of_lists = count_lists(appdata);
	lists = malloc(sizeof(t_list) * num_of_lists);
	current = appdata->first_token;
	while (++i < num_of_lists)
	{
		while (is_list_end(current) == 0)
			current = current->next;
		end_pos = current->pos;
		lists[i] = make_a_list(appdata, start_pos, end_pos);
		start_pos = end_pos + 1;
		current = current->next;
	}
	appdata->lists_num = num_of_lists;
	appdata->lists = lists;
}







// void  fill_the_lists(t_appdata *appdata)
// {
// 	int	i;
// 	int	start_pos;
// 	int	end_pos;
// 	t_token	*current;
	
// 	i = -1;
// 	start_pos = 0;
// 	current = appdata->first_token;
// 	while (++i < appdata->lists_num)
// 	{
// 		while (is_list_end(current) == 0)
// 			current = current->next;
// 		end_pos = current->pos;
// 		printf("Going for the list # %i\n", i);
// 		make_a_list(appdata, start_pos, end_pos, i);
// 		printf("Made a list # %i\n", i);
// 		start_pos = end_pos + 1;
// 		end_pos = start_pos;
// 	}
// }

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
