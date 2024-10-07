/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:14:11 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/10/03 16:51:14 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_type_of_relative_path(t_cmd_token *token)
{
	size_t	len;

	len = ft_strlen(token->argv[1]);
	if (ft_strncmp(token->argv[1], ".", len) == 0)
		return (1);
	else if (ft_strncmp(token->argv[1], "..", len) == 0)
		return (2);
	else if (ft_strncmp(token->argv[1], "--", len) == 0)
		return (3);
	else if (ft_strncmp(token->argv[1], "~", len) == 0)
		return (4);
	else if (ft_strncmp(token->argv[1], "../", 3) == 0)
		return (5);
	else if (ft_strncmp(token->argv[1], "/", 1) != 0)
		return (6);
	return (0);
}

int	update_envs(char *cwd, t_env *env)
{
	char	new_pwd[1024];

	if (getcwd(new_pwd, 1024) == NULL)
		return (perror("Error"), 1);
	free_and_renew_env_value(env, "OLDPWD", cwd);
	free_and_renew_env_value(env, "PWD", new_pwd);
	return (0);
}

char	*relative_path_preparation(t_cmd_token *token, t_env *env)
{
	char	*path;
	int		type;

	path = NULL;
	type = get_type_of_relative_path(token);
	if (type == 1 || type == 2 || type == 5 || type == 6)
		path = ft_strdup(token->argv[1]);
	else if (type == 3)
	{
		path = ft_get_env(env, "OLDPWD");
		if (!path)
			return (NULL);
	}
	else if (type == 4)
	{
		path = ft_get_env(env, "HOME");
		if (!path)
			return (NULL);
	}
	return (path);
}

int	ft_cd(t_cmd_token *token, t_env *env)
{
	char	cwd[1024];
	char	*path;

	path = NULL;
	if (token->argc == 1)
		return (ft_putstr_fd(CD_ONE_ARG_ERROR, 2), 1);
	else if (token->argc > 2)
		return (ft_putstr_fd(CD_TOO_MANY_ARG_ERROR, 2), 1);
	if (getcwd(cwd, 1024) == NULL)
		return (perror("Error"), 2);
	if (token->argv[1] && token->argv[1][0] == '/')
		path = ft_strdup(token->argv[1]);
	else if (token->argv[1] && token->argv[1][0] != '/')
	{
		path = relative_path_preparation(token, env);
		if (!path)
			return (perror("Error"), 3);
	}
	if (chdir(path) == -1)
		return (perror("Error"), 4);
	if (get_type_of_relative_path(token) != 1)
		update_envs(cwd, env);
	free(path);
	return (0);
}
