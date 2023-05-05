/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 13:47:34 by francisco         #+#    #+#             */
/*   Updated: 2023/05/05 13:48:04 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	close_pipe(t_data **d)
{
	close((*d)->fd[0]);
	close((*d)->fd[1]);
}

void	handle_pipes(t_data **d, t_ints *n)
{
	while ((*d)->tokens[n->i].token && (*d)->tokens[n->i].f_pipe == 0)
		n->i++;
	if (n->j == 0)
	{
		dup2((*d)->fd[1], STDOUT_FILENO);
	}
	else if (n->j > 0 && n->j < (*d)->num_commands - 1)
	{
		dup2((*d)->fd[1], STDOUT_FILENO);
		dup2((*d)->old_fd, STDIN_FILENO);
	}
	else if (n->j == (*d)->num_commands - 1)
	{
		dup2((*d)->old_fd, STDIN_FILENO);
	}
	close_pipe(d);
	n->i++;
}
