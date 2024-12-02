/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:05:54 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/12/02 18:58:09 by vkinsfat         ###   ########.fr       */
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
			if (!cmd->argv[i])
				return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
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

void	set_std_redirection(t_cmd *cmd, t_token *current, int *input_i, int *output_i)
{
	if (current->type == STDIN)
	{
		if (*input_i >= cmd->num_of_infiles)
			return ;
		cmd->input_redir_type[*input_i] = STDIN;
		cmd->infile_name[*input_i] = ft_strdup(current->next->value);
		cmd->infile_name[*input_i + 1] = NULL;
		(*input_i)++;
	}
	else if (current->type == STDOUT)
	{
		cmd->output_redir_type[*output_i] = STDOUT;
		cmd->outfile_name[*output_i] = ft_strdup(current->next->value);
		cmd->outfile_name[*output_i + 1] = NULL;
		(*output_i)++;
	}
}

void	count_amount_of_redirections(t_cmd *cmd, t_token *current)
{
	while (is_cmd_end(current) == FALSE && current->next != NULL)
	{
		if (current->type == STDIN || current->type == HEREDOC)
		{
			if (current->type == HEREDOC)
				cmd->num_of_delims++;
			cmd->num_of_infiles++;
		}
		if (current->type == STDOUT || current->type == APPEND)
			cmd->num_of_outfiles++;
		current = current->next;
	}
}

void	init_redirections_in_cmd(t_cmd *cmd)
{
	if (cmd->num_of_infiles > 0)
	{
		cmd->input_redir_type = malloc(sizeof(int) * cmd->num_of_infiles);
		cmd->infile_name = malloc(sizeof(char *) * (cmd->num_of_infiles + 1));
	}
	if (cmd->num_of_outfiles > 0)
	{
		cmd->output_redir_type = malloc(sizeof(int) * cmd->num_of_outfiles);
		cmd->outfile_name = malloc(sizeof(char *) * (cmd->num_of_outfiles + 1));
	}
	if (cmd->num_of_delims > 0)
		cmd->delim = malloc(sizeof(char *) * (cmd->num_of_delims + 1));
}

void	set_redirections_in_cmd(t_cmd *cmd, t_token *current)
{
	int	input_i;
	int	output_i;
	int	heredoc_i;

	input_i = 0;
	output_i = 0;
	heredoc_i = 0;
	count_amount_of_redirections(cmd, current);
	init_redirections_in_cmd(cmd);
	while (is_cmd_end(current) == FALSE && current->next != NULL)
	{
		if (current->type >= STDIN && current->type <= APPEND)
		{
			if (current->type == STDIN || current->type == STDOUT)
				set_std_redirection(cmd, current, &input_i, &output_i);
			else if (current->type == HEREDOC)
			{
				cmd->input_redir_type[input_i] = HEREDOC;
				cmd->infile_name[input_i] = ft_strdup("here_doc.txt");
				cmd->delim[heredoc_i] = ft_strdup(current->next->value);
				cmd->delim[heredoc_i + 1] = NULL;
				input_i++;
				heredoc_i++;
			}
			else if (current->type == APPEND)
			{
				cmd->output_redir_type[output_i] = APPEND;
				cmd->outfile_name[output_i] = ft_strdup(current->next->value);
				output_i++;
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
