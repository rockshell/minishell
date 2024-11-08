/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:55:16 by akulikov          #+#    #+#             */
/*   Updated: 2024/11/08 18:12:07 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_cmd(t_cmd *cmd, t_token *first, t_token *last, int is_pipe_before)
{
	cmd->argc = 0;
	cmd->input_redir_type = 0;
	cmd->output_redir_type = 0;
	cmd->num_of_infiles = 0;
	cmd->num_of_outfiles = 0;
	cmd->num_of_delims = 0;
	cmd->is_pipe_before = 0;
	cmd->is_pipe_after = 0;
	cmd->argv = NULL;
	cmd->infile_name = NULL;
	cmd->outfile_name = NULL;
	cmd->delim = NULL;
	set_pipes_in_cmd(cmd, is_pipe_before, last);
	set_redirections_in_cmd(cmd, first);
	if (set_the_command_itself(cmd, first) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

//TODO - deal with overallocate
int	init_the_list(t_list *list, int start, int end)
{
	list->size = 0;
	list->and_after = 0;
	list->or_after = 0;
	list->end_after = 0;
	list->cmd = malloc(sizeof(t_cmd) * (end - start));
	if (!list->cmd)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	list->exec_data = malloc(sizeof(t_exec_data));
	if (!list->exec_data)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	init_exec_data(list);
	return (SUCCESS);
}

int	clean_the_quotes(t_token *token)
{
	char	*unquoted_value;
	int		len;

	while (token)
	{
		if (is_contain_quotes(token) == TRUE)
		{
			if (ft_strchr(token->value, '$'))
				handle_env_quotes(token);
			else
			{
				len = count_quoted_len(token);
				unquoted_value = malloc(sizeof(char) * (len + 1));
				if (!unquoted_value)
					return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
				no_quote_copy(token, unquoted_value);
				free(token->value);
				token->value = ft_strdup(unquoted_value);
				if (!token->value)
					return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
				free(unquoted_value);
			}
		}
		token = token->next;
	}
	return (SUCCESS);
}

int	make_commands(t_appdata *appdata, t_list *list, int start, int end)
{
	int		j;
	int		is_pipe_before_flag;
	t_token	*cmd_start;
	t_token	*cmd_end;

	j = 0;
	is_pipe_before_flag = 0;
	cmd_start = &appdata->tokens[start];
	cmd_end = cmd_start;
	while (cmd_end && cmd_end->pos <= end)
	{
		while (is_cmd_end(cmd_end) == 0 && cmd_end->next != NULL)
			cmd_end = cmd_end->next;
		if (init_cmd(&list->cmd[j++], cmd_start, cmd_end, is_pipe_before_flag) == FAILURE)
			return (FAILURE);
		if (cmd_end->type == PIPE)
			is_pipe_before_flag = 1;
		list->size++;
		if (cmd_end->next)
		{
			cmd_start = cmd_end->next;
			cmd_end = cmd_start;
		}
		else
			break ;
	}
	return (SUCCESS);
}

int	make_a_list(t_appdata *appdata, t_list *list, int start, int end)
{
	if (init_the_list(list, start, end) == FAILURE)
		return (FAILURE);
	if (make_commands(appdata, list, start, end) == FAILURE)
		return (FAILURE);
	if (appdata->tokens[end].type == LOGICAL_AND)
		list->and_after = 1;
	else if (appdata->tokens[end].type == LOGICAL_OR)
		list->or_after = 1;
	else
		list->end_after = 1;
	return (SUCCESS);
}

int	make_lists(t_appdata *appdata)
{
	int		i;
	int		start_pos;
	int		end_pos;
	t_token	*current;

	i = -1;
	start_pos = 0;
	current = appdata->first_token;
	while (++i < appdata->lists_num)
	{
		while (is_list_end(current) == FALSE)
			current = current->next;
		end_pos = current->pos;
		if (make_a_list(appdata, &appdata->lists[i], start_pos, end_pos) == 1)
			return (FAILURE);
		start_pos = end_pos + 1;
		current = current->next;
	}
	return (SUCCESS);
}

int	run_lexer(t_appdata *appdata)
{
	if (!appdata->first_token)
		return (SUCCESS);
	if (syntax_check(appdata->first_token) == FALSE)
	{
		appdata->exit_code = 2;
		return (FAILURE);
	}
	check_if_env(appdata->first_token);
	if (clean_the_quotes(appdata->first_token) == FAILURE)
		return (FAILURE);
	expand_tokens(appdata->first_token, appdata->env, appdata->exit_status);
	appdata->lists_num = count_lists(appdata);
	appdata->lists = malloc(sizeof(t_list) * appdata->lists_num);
	if (!appdata->lists)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	if (make_lists(appdata) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
