/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:14:58 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/11 14:40:58 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_no_args(t_env *env)
{
	char	**array_of_keys;
	char	*value;
	int		array_len;
	int		i;

	array_len = get_length_of_env(env);
	array_of_keys = get_an_array_of_keys(env, array_len);
	if (!array_of_keys)
		return (1);
	sort_an_array(array_of_keys, array_len);
	i = 0;
	while (i < array_len)
	{
		value = ft_get_env(env, array_of_keys[i]);
		if (value)
			printf("declare -x %s=\"%s\"\n", array_of_keys[i], value);
		else
			printf("declare -x %s=''\n", array_of_keys[i]);
		free(value);
		free(array_of_keys[i]);
		i++;
	}
	free(array_of_keys);
	return (0);
}

int	change_value(t_env *env, char *argument)
{
	t_env	*temp;
	char	*key;

	if (ft_strchr(argument, '=') == NULL)
		return (0);
	key = get_key(argument);
	if (!key)
		return (1);
	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			free(temp->value);
			temp->value = get_value(argument);
		}
		temp = temp->next;
	}
	free(key);
	return (0);
}

int	export_with_args(t_cmd_token *token, t_env *env)
{
	int	i;
	int	exit;

	i = 1;
	exit = 0;
	while (i < token->argc)
	{
		if (is_valid_var(token->argv[i]) == 0)
		{
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(token->argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			exit = 1;
		}
		else
		{
			if (is_in_var(env, token->argv[i]) == 0)
				create_env_var_node(&env, token->argv[i]);
			else
				change_value(env, token->argv[i]);
		}
		i++;
	}
	return (exit);
}

int	ft_export(t_cmd_token *token, t_env *env)
{
	int	exit;

	exit = 0;
	if (token->argc == 1)
		exit = export_no_args(env);
	else if (token->argc > 1)
		exit = export_with_args(token, env);
	return (exit);
}
