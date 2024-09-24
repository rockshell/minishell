/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akulikov <akulikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:13:56 by akulikov          #+#    #+#             */
/*   Updated: 2024/09/24 16:28:03 by akulikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_type_of_token(char *command)
{
	if (ft_strncmp(command, "|", ft_strlen(command)) == 0)
		return (1);
	else if (ft_strncmp(command, "<", ft_strlen(command)) == 0)
		return (2);
	else if (ft_strncmp(command, ">", ft_strlen(command)) == 0)
		return (3);
	else if (ft_strncmp(command, "<<", ft_strlen(command)) == 0)
		return (4);
	else if (ft_strncmp(command, ">>", ft_strlen(command)) == 0)
		return (5);
	return(0);
}
