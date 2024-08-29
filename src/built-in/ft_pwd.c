/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:14:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/08/29 18:28:47 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_pwd(t_appdata *appdata)
{
    char *pwd;
    int i;

    i = 0;
    while (appdata->env[i])
    {
        if (ft_strncmp(appdata->env[i], "PWD=", 4) == 0)
            pwd = strdup(appdata->env[i] + 4);
        else 
            i++;
    }
    printf("%s\n", pwd);
    free(pwd);
}
