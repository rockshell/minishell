/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:05:54 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/31 20:33:12 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_argc(t_cmd *cmd, t_token *first)
{
	t_token	*current;

	current = first;
	while (is_cmd_end(current) == FALSE)
	{
		if (current->type == WORD && current->is_parsed == 0)
			cmd->argc++;
		if (current->next)
			current = current->next;
		else
			break ;
	}
}

int	set_the_command_itself(t_cmd *cmd, t_token *first)
{
	int		i;
	t_token	*current;

	set_argc(cmd, first);
	cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
	if (!cmd->argv)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	current = first;
	i = 0;
	while (is_cmd_end(current) == FALSE)
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
			break ;
	}
	cmd->argv[i] = NULL;
	return (SUCCESS);
}

void	set_std_redirection(t_cmd *cmd, t_token *current)
{
	if (current->type == STDIN)
	{
		cmd->input_redir_type = STDIN;
		cmd->infile_name = ft_strdup(current->next->value);
	}
	else if (current->type == STDOUT)
	{
		cmd->output_redir_type = STDOUT;
		cmd->outfile_name = ft_strdup(current->next->value);
	}
}

void	set_redirections_in_cmd(t_cmd *cmd, t_token *current)
{
	while (is_cmd_end(current) == FALSE && current->next != NULL)
	{
		if (current->type >= STDIN && current->type <= APPEND)
		{
			if (current->type == STDIN || current->type == STDOUT)
				set_std_redirection(cmd, current);
			else if (current->type == HEREDOC)
			{
				cmd->input_redir_type = HEREDOC;
				cmd->delim = ft_strdup(current->next->value);
			}
			else if (current->type == APPEND)
			{
				cmd->output_redir_type = APPEND;
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
	if (pipe_flag == TRUE)
		cmd->is_pipe_before = 1;
	if (last->type == PIPE)
	{
		cmd->is_pipe_after = 1;
		last->is_parsed = 1;
	}
}
