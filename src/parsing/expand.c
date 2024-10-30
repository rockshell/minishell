/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 22:36:37 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/30 18:46:55 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_no_env_string(char *value, int i)
{
	int		j;
	char	*temp;
	char	*new_value;

	j = i;
	temp = NULL;
	new_value = NULL;
	while (value[j] != '$')
		j++;
	if (j > i)
	{
		temp = ft_substr(value, i, j);
		if (!temp)
			return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		new_value = gnl_strjoin(new_value, temp);
		if (!new_value)
			return (free(temp), ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		free(temp);
		return (new_value);
	}
	return (NULL);
}

char	*update_result(char *temp, char *new_value)
{
	if (temp == NULL)
		return (NULL);
	else
	{
		if (new_value == NULL)
		{
			new_value = ft_strdup(temp);
			if (!new_value)
				return (free(temp), ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		}
		else
		{
			new_value = gnl_strjoin(new_value, temp);
			if (!new_value)
				return (free(temp), ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		}
		free(temp);
	}
	return (new_value);
}

char	*get_expanded_string(char *value, int i, t_env *env, t_env *exit_status)
{
	char	*temp;
	char	*substr;
	int		j;

	j = i;
	temp = NULL;
	substr = NULL;
	while (ft_isspace(value[j]) == 0 && value[j] != '$' && value[j] != '\0')
		j++;
	substr = ft_substr(value, i, (size_t)(j - i));
	if (!substr)
		return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
	temp = expand_env_var(substr, env, exit_status);
	if (!temp)
		return (free(substr), NULL);
	free(substr);
	return (temp);
}

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
		temp = get_expanded_string(value, i, env, exit_status);
		printf("kek\n");
		new_value = gnl_strjoin(new_value, temp);
		printf("New value is: %s\n", new_value);
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
