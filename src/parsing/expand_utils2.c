/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 20:58:41 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/31 20:59:38 by vitakinsfat      ###   ########.fr       */
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

char	*get_expanded_str(char *value, int *i, t_env *env, t_env *exit_status)
{
	char	*temp;
	char	*substr;
	int		j;

	j = *i;
	temp = NULL;
	substr = NULL;
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
