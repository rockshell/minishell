/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:17:26 by vkinsfat          #+#    #+#             */
/*   Updated: 2024/09/02 14:57:21 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	print_nodes(t_env *env)
// {
// 	t_env	*temp;

// 	temp = env;
// 	while (temp)
// 	{
// 		printf("Key: %s", temp->key);
// 		printf("   Value: %s\n", temp->value);
// 		temp = temp->next;
// 	}
// }

// int	main(int ac, char **av, char **envp)
// {
// 	int	i;
// 	t_appdata *appdata;

// 	(void)ac;
// 	(void)av;
// 	appdata = malloc(sizeof(t_appdata));
// 	if (!appdata)
// 		return (ft_putstr_fd(ALLOC_ERROR_MSG, 2), 1);
// 	appdata->env = NULL;
// 	i = 0;
// 	while (envp[i])
// 	{
// 		create_node(&(appdata->env), envp[i]);
// 		i++;
// 	}
// 	// print_nodes(appdata->env);
// 	return (0);
// }

int	main()
{
	char		*input;
	t_appdata	appdata;

	while (1)
	{
		input = readline("minishell: ");
		initial_parsing(input, &appdata);
	}
	return (0);
}
