/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:13:56 by akulikov          #+#    #+#             */
/*   Updated: 2024/10/22 18:01:29 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_the_command_itself(t_cmd *cmd, t_token *first)
{
	int	i;
	t_token *current;

	current = first;
	while (is_cmd_end(current) == 0)
	{
		if (current->type == WORD && current->is_parsed == 0)
			cmd->argc++;
		if (current->next)
			current = current->next;
		else
			break;
	}
	cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
	current = first;
	i = 0;
	while (is_cmd_end(current) == 0)
	{
		if (current->type == WORD && current->is_parsed == 0)
		{
			cmd->argv[i] = ft_strdup(current->value);
			current->is_parsed = 1;
			i++;
		}
		if (current->next)
			current = current->next;
		else
			break;
	}
	cmd->argv[i] = NULL;
}

//TODO - rat things clear
void	set_redirections_in_cmd(t_cmd *cmd, t_token *current)
{
	while (is_cmd_end(current) == 0 && current->next != NULL)
	{
		if (current->type >=3 && current->type <= 6)
		{
			if (current->type == 3)
			{
				cmd->input_redir_type = 3;
				cmd->infile_name = ft_strdup(current->next->value);
			}
			else if(current->type == 4)
			{
				cmd->output_redir_type = 4;
				cmd->outfile_name = ft_strdup(current->next->value);
			}
			else if(current->type == 5)
			{
				cmd->input_redir_type = 5;
				cmd->delim = ft_strdup(current->next->value);
			}
			else if (current->type == 6)
			{
				cmd->output_redir_type = 6;
				cmd->outfile_name = ft_strdup(current->next->value);
			}
			current->is_parsed = 1;
			current->next->is_parsed = 1;
		}
		current = current->next;
	}
}

void	set_pipes_in_cmd(t_cmd *cmd, int pipe_flag, t_token *last)
{
	if (pipe_flag == 1)
		cmd->is_pipe_before = 1;
	if (last->type == 2)
	{
		cmd->is_pipe_after = 1;
		last->is_parsed = 1;
	}
}

int	count_lists(t_appdata *appdata)
{
	int	res;
	t_token *current;
	
	res = 1;
	current = appdata->first_token;
	while (current)
	{
		if (current->type == 7 || current->type == 8)
			res++;
		current = current->next;
	}
	return (res);
}

int	is_cmd_end(t_token *token)
{
	if (token->type == 2)
		return (1); 
	if (token->type == 7 || token->type == 8)
		return (1);
	return (0);
}

int	is_list_end(t_token *token)
{
	if (token->next == NULL)
		return (1);
	if (token->type == 7 || token->type == 8)
		return (1);
	return (0);
}

int	is_token_redirection(t_token *token)
{
	if (token->type > 2 && token->type < 7)
		return (1);
	return (0);
}
