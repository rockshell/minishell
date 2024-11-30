/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arch <arch@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 22:36:37 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/11/30 16:48:46 by arch             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_token(t_token *current, t_env *env, t_env *exit_status)
{
	int		i;
	int		j;
	int		quotes_flag;
	char	*new_value;
	char	*temp;
	char	*key;

	i = 0;
	j = 0;
	quotes_flag = 0;
	temp = NULL;
	new_value = NULL;
	key = NULL;
	while (current->value[i])
	{
		if (current->value[i] == '\'')
			quotes_flag = !quotes_flag;
		if (current->value[i] == '$' && current->value[i + 1] && 
			is_valid_env_var_first_symbol(current->value[i + 1]) && !quotes_flag)
		{
			// printf("Current j is %d and i is %d\n", j, i);
			// temp = ft_substr(current->value, j, i - j);
			// new_value = expand_strjoin(new_value, temp);
			// printf("Temp: %s\n", temp);
			// printf("New value: %s\n", new_value);
			// free(temp);
			j = i;
			i++;
			while (current->value[i] && 
				is_valid_symbol_for_env_var_name(current->value[i]))
			{
				if (current->value[i] == '?')
				{
					i++;
					break;
				}
				i++;
			}
			key = ft_substr(current->value, j, i - j);
			temp = expand_env_var(key, env, exit_status);
			new_value = expand_strjoin(new_value, temp);
			// printf("Key: %s\n", key);
			// printf("Current temp: %s\n", temp);
			// printf("Current new value: %s\n", new_value);
			free(key);
			free(temp);
			j = i;
		}
		else
		{
			new_value = append_char(new_value, current->value[i]);
			// printf("New value: %s\n", new_value);
			i++;
		}
	}
	free(current->value);
	current->value = ft_strdup(new_value);
	free(new_value);
	return (SUCCESS);
}

int	new_expand_tokens(t_token *first_token, t_env *env, t_env *exit_status)
{
	t_token *current;

	current = first_token;
	while (current)
	{
		if (current->value && ft_strchr(current->value, '$'))
			expand_token(current, env, exit_status);
		current = current->next;
	}
	return(SUCCESS);
}
