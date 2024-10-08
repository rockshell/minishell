/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:13:56 by akulikov          #+#    #+#             */
/*   Updated: 2024/10/08 19:36:04 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
