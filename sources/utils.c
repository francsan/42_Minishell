/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:26:23 by francisco         #+#    #+#             */
/*   Updated: 2023/05/05 14:30:17 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	get_paths(t_data **d)
{
	int	i;

	i = -1;
	while ((*d)->env[++i])
	{
		if (ft_strncmp((*d)->env[i], "PATH=", 5) == 0)
		{
			(*d)->paths = ft_split(&(*d)->env[i][5], ':');
			break ;
		}
	}
}

void	free_all(t_data **d)
{
	int	i;

	i = -1;
	while ((*d)->tokens[++i].token)
		free((*d)->tokens[i].token);
	free((*d)->tokens);
	ft_strarr_free((*d)->paths);
	ft_strarr_free((*d)->env);
	free(d);
}
