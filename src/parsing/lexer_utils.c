/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:13:56 by akulikov          #+#    #+#             */
/*   Updated: 2024/10/09 17:10:48 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_pipes_in_cmd(t_appdata *appdata, t_cmd *cmd, int first, int last)
{
	cmd->is_pipe_before = 0;
	cmd->is_pipe_after = 0;
	if (appdata->tokens[first].type == 2)
		cmd->is_pipe_before = 1;
	if (appdata->tokens[first].type == 2)
		cmd->is_pipe_after = 1;
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
	if (token->next != NULL || token->type != 2)
		return (1); 
	if (token->type != 7 || token->type != 8)
		return (1);
	return (0);
}

int	is_list_end(t_token *token)
{
	if (token == NULL)
		return (1);
	if (token->type == 7 || token->type == 8)
		return (1);
	return (0);
}

// int	get_type_of_string(char *string)
// {
// 	if (ft_strncmp(string, "|", ft_strlen(string)) == 0)
// 		return (PIPE);
// 	else if (ft_strncmp(string, "<", ft_strlen(string)) == 0)
// 		return (IN_REDIR);
// 	else if (ft_strncmp(string, ">", ft_strlen(string)) == 0)
// 		return (OUT_REDIR);
// 	else if (ft_strncmp(string, "<<", ft_strlen(string)) == 0)
// 		return (HERE_DOC_REDIR);
// 	else if (ft_strncmp(string, ">>", ft_strlen(string)) == 0)
// 		return (APPEND_REDIR);
// 	return(WORD);
// }