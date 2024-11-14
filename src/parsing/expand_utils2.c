/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkinsfat <vkinsfat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:58:41 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/14 19:01:32 by vkinsfat         ###   ########.fr       */
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
		temp = ft_substr(value, i, j - i);
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
	if (!temp && !new_value)
		return (NULL);
	if (temp == NULL && new_value)
		return (ft_strdup(new_value));
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

char	*get_end_of_str(char *value, int *i, char *temp)
{
	char	*substr;
	char	*result;
	int		j;

	j = *i;
	substr = NULL;
	result = NULL;
	while (value[j] && value[j] != '$')
		j++;
	if (j > *i)
	{
		substr = ft_substr(value, *i, j - *i);
		if (!substr)
			return (free(temp), ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		result = ft_strjoin(temp, substr);
		free(temp);
		free(substr);
		if (!result)
			return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
		*i = j;
	}
	*i = j;
	return (result);
}

char	*expand_exit_status(char *value, int *i, t_env *env, t_env *exit_status)
{
	char	*temp;
	char	*substr;
	char	*result;
	int		j;

	j = *i;
	temp = NULL;
	substr = NULL;
	result = NULL;
	substr = ft_substr(value, *i, 1);
	if (!substr)
		return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
	temp = expand_env_var(substr, env, exit_status);
	if (!temp)
		return (free(substr), NULL);
	free(substr);
	*i = j + 1;
	if (value[j + 1])
	{
		result = get_end_of_str(value, i, temp);
		if (!result)
			return (NULL);
		return (result);
	}
	return (temp);
}

char	*get_expanded_str(char *value, int *i, t_env *env, t_env *exit_status)
{
	char	*temp;
	char	*substr;
	int		j;

	j = *i;
	temp = NULL;
	substr = NULL;
	if (value[j] == '?')
	{
		temp = expand_exit_status(value, i, env, exit_status);
		if (!temp)
			return (NULL);
		return (temp);
	}
	while (ft_isspace(value[j]) == 0 && value[j] != '$' && value[j] != '\0')
		j++;
	substr = ft_substr(value, *i, (size_t)(j - *i));
	if (!substr)
		return (ft_putstr_fd(ALLOC_ERROR, 2), NULL);
	temp = expand_env_var(substr, env, exit_status);
	if (!temp)
		return (free(substr), NULL);
	free(substr);
	*i = j;
	return (temp);
}
