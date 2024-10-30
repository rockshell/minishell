/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 23:38:25 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/30 18:53:02 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_contain_quotes(t_token *token)
{
	if (ft_strchr(token->value, '"') != NULL)
		return (TRUE);
	else if (ft_strchr(token->value, '\'') != NULL)
		return (TRUE);
	else
		return (FALSE);
}

int	is_quotes_double(t_token *token)
{
	int	i;

	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '"' )
			return (TRUE);
		else if (token->value[i] == '\'')
			return (FALSE);
		i++;
	}
	return (-1);
}

int	count_quoted_len(t_token *token)
{
	int	i;
	int	len;
	int	is_double;

	i = 0;
	len = 0;
	is_double = is_quotes_double(token);
	while (token->value[i])
	{
		if (token->value[i] == '"' && is_double == TRUE)
			i++;
		else if (token->value[i] == '\'' && is_double == FALSE)
			i++;
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}

void	no_quote_copy(t_token *token, char *str)
{
	int	i;
	int	j;
	int	is_double;

	i = 0;
	j = 0;
	is_double = is_quotes_double(token);
	while (token->value[i])
	{
		if (token->value[i] == '"' && is_double == TRUE)
			i++;
		else if (token->value[i] == '\'' && is_double == FALSE)
			i++;
		else
			str[j++] = token->value[i++];
	}
	str[j] = '\0';
}

void	handle_env_quotes(t_appdata *appdata, t_token *token)
{
	char	*unquoted_value;
	int		len;
	int		is_double;

	len = count_quoted_len(token);
	is_double = is_quotes_double(token);
	unquoted_value = malloc(sizeof(char) * (len + 1));
	if (!unquoted_value)
		error_rising(appdata);
	no_quote_copy(token, unquoted_value);
	free(token->value);
	token->value = ft_strdup(unquoted_value);
	if (!token->value)
		error_rising(appdata);
	free(unquoted_value);
	if (is_double == FALSE)
		token->needs_expanding = 0;
}
