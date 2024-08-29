/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vitakinsfator <vitakinsfator@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:15:32 by vitakinsfat       #+#    #+#             */
/*   Updated: 2024/08/29 18:24:33 by vitakinsfat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_env(t_appdata *appdata)
{
    int i;

    i = -1;
    while (appdata->env[++i])
        printf("%s\n", env[i]);
}
