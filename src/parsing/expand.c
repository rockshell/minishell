/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 22:36:37 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/31 20:59:43 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	complex_expanding(t_token *token, t_env *env, t_env *exit_status)
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
		while (value[i] != '$')
			i++;
		i++;
		temp = get_expanded_str(value, &i, env, exit_status);
		new_value = gnl_strjoin(new_value, temp);
		free(temp);
	}
	free(token->value);
	token->value = ft_strdup(new_value);
	free(new_value);
}

void	simple_expanding(t_token *token, t_env *env, t_env *exit_status)
{
	char	*value;
	char	*result;

	value = token->value;
	value++;
	result = expand_env_var(value, env, exit_status);
	free(token->value);
	token->value = ft_strdup(result);
	free(result);
}

void	expand_single_token(t_token *token, t_env *env, t_env *exit_status)
{
	int	num_of_expandable;

	num_of_expandable = count_expandables(token->value);
	if (num_of_expandable == 1 && no_sep(token->value) == TRUE
		&& token->value[0] == '$')
		simple_expanding(token, env, exit_status);
	else
		complex_expanding(token, env, exit_status);
}

void	expand_tokens(t_token *first_token, t_env *env, t_env *exit_status)
{
	t_token	*current;

	current = first_token;
	while (current)
	{
		if (current->needs_expanding)
		{
			expand_single_token(current, env, exit_status);
			current->needs_expanding = 0;
		}
		current = current->next;
	}
}
