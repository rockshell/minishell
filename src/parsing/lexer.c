/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:55:16 by akulikov          #+#    #+#             */
/*   Updated: 2024/10/29 23:52:45 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_cmd(t_cmd *cmd, t_token *first_token, t_token *last_token, int is_pipe_before_flag)
{
	cmd->argc = 0;
	cmd->input_redir_type = 0;
	cmd->output_redir_type = 0;
	cmd->is_pipe_before = 0;
	cmd->is_pipe_after = 0;
	cmd->argv = NULL;
	cmd->infile_name = NULL;
	cmd->outfile_name = NULL;
	cmd->delim = NULL;
	set_pipes_in_cmd(cmd, is_pipe_before_flag, last_token);
	set_redirections_in_cmd(cmd, first_token);
	set_the_command_itself(cmd, first_token);
}

t_list	init_the_list(int start, int end)
{
	t_list	list;

	list.size = 0;
	list.and_after = 0;
	list.or_after = 0;
	list.end_after = 0;
	list.cmd = malloc(sizeof(t_cmd) * (end - start)); //TODO - deal with overallocate 
	list.exec_data = malloc(sizeof(t_exec_data));
	init_exec_data(&list);
	return (list);
}

void	clean_the_quotes(t_appdata *appdata, t_token *token)
{
	char	*unquoted_value;
	int		len;

	while (token)
	{
		if (is_contain_quotes(token) == TRUE)
		{
			if (ft_strchr(token->value, '$'))
				handle_env_quotes(appdata, token);
			else
			{
				len = count_quoted_len(token);
				unquoted_value = malloc(sizeof(char) * (len + 1));
				if (!unquoted_value)
					error_rising(appdata);
				no_quote_copy(token, unquoted_value);
				free(token->value);
				token->value = ft_strdup(unquoted_value);
				if (!token->value)
					error_rising(appdata);
				free(unquoted_value);
			}
		}
		token = token->next;
	}
}

void check_if_env(t_token *token)
{
	t_token *temp;

	temp = token;
	while (temp)
	{
		if (ft_strchr(temp->value, '$'))
			temp->needs_expanding = 1;
		temp = temp->next;
	}
}

//TODO - return a pointer instead of struct
t_list	make_a_list(t_appdata *appdata, int start, int end)
{
	int	j;
	int	is_pipe_before_flag;
	t_list	list;
	t_token	*cmd_start;
	t_token	*cmd_end;
	
	j = 0;
	is_pipe_before_flag = 0;
	cmd_start = &appdata->tokens[start];
	cmd_end = cmd_start;
	list = init_the_list(start, end);
	check_if_env(appdata->first_token);
	clean_the_quotes(appdata, appdata->first_token);
	// print_tokens(appdata);
	expand_tokens(appdata->first_token, appdata->env, appdata->exit_status);
	while (cmd_end && cmd_end->pos <= end)
	{
		while (is_cmd_end(cmd_end) == 0 && cmd_end->next != NULL)
			cmd_end = cmd_end->next;
		init_cmd(&list.cmd[j++], cmd_start, cmd_end, is_pipe_before_flag);
		if (cmd_end->type == 2)
			is_pipe_before_flag = 1;
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
	if (syntax_check(appdata->first_token) == FALSE)
	{
		appdata->exit_code = 2;
		return ;
	}
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
