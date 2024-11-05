/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 22:36:37 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/05 19:10:24 by vkinsfat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	complex_expanding(t_token *token, t_env *env, t_env *exit_status)
{
	int		i;
	char	*temp;
	char	*value;
	char	*new_value;
	size_t	len;

	value = token->value;
	len = ft_strlen(value);
	i = 0;
	temp = NULL;
	new_value = NULL;
	while (i < (int)len)
	{
		temp = get_no_env_string(value, i);
		new_value = update_result(temp, new_value);
		while (i < (int)len && value[i] != '$')
			i++;
		i++;
		temp = get_expanded_str(value, &i, env, exit_status);
		new_value = gnl_strjoin(new_value, temp);
		if (!new_value)
			return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
		free(temp);
	}
	free(token->value);
	token->value = ft_strdup(new_value);
	if (!token->value)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	free(new_value);
	return (SUCCESS);
}

int	simple_expanding(t_token *token, t_env *env, t_env *exit_status)
{
	char	*value;
	char	*result;

	value = token->value;
	value++;
	if (*value == '?' && ft_strlen(value) != 1)
	{
		if (complex_expanding(token, env, exit_status) == FAILURE)
			return (FAILURE);
		return (SUCCESS);
	}
	result = expand_env_var(value, env, exit_status);
	if (!result)
		return (FAILURE);
	free(token->value);
	token->value = ft_strdup(result);
	if (!token->value)
		return (ft_putstr_fd(ALLOC_ERROR, 2), FAILURE);
	free(result);
	return (SUCCESS);
}

int	expand_single_token(t_token *token, t_env *env, t_env *exit_status)
{
	int	num_of_expandable;

	num_of_expandable = count_expandables(token->value);
	if (num_of_expandable == 1 && no_sep(token->value) == TRUE
		&& token->value[0] == '$')
	{
		if (simple_expanding(token, env, exit_status) == FAILURE)
			return (FAILURE);
	}
	else
	{
		if (complex_expanding(token, env, exit_status) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	expand_tokens(t_token *first_token, t_env *env, t_env *exit_status)
{
	t_token	*current;

	current = first_token;
	while (current)
	{
		if (current->needs_expanding)
		{
			if (expand_single_token(current, env, exit_status) == FAILURE)
				return (FAILURE);
			current->needs_expanding = 0;
		}
		current = current->next;
	}
	return (SUCCESS);
}
