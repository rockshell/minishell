/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:13:56 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/26 19:42:17 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_type_of_string(char *string)
{
	if (ft_strncmp(string, "|", ft_strlen(string)) == 0)
		return (PIPE);
	else if (ft_strncmp(string, "<", ft_strlen(string)) == 0)
		return (IN_REDIR);
	else if (ft_strncmp(string, ">", ft_strlen(string)) == 0)
		return (OUT_REDIR);
	else if (ft_strncmp(string, "<<", ft_strlen(string)) == 0)
		return (HERE_DOC_REDIR);
	else if (ft_strncmp(string, ">>", ft_strlen(string)) == 0)
		return (APPEND_REDIR);
	return(WORD);
}
