/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 13:47:34 by francisco         #+#    #+#             */
/*   Updated: 2023/05/19 15:30:16 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	close_pipe(t_data **d)
{
	close((*d)->fd[0]);
	close((*d)->fd[1]);
}

void	check_for_redirs(t_data **d, t_ints *n, int *f_input, int *f_output)
{
	*f_input = 0;
	*f_output = 0;
	while ((*d)->tokens[n->i].token && (*d)->tokens[n->i].f_pipe == 0)
	{
		if ((*d)->tokens[n->i].f_redir_input)
			*f_input = 1;
		if ((*d)->tokens[n->i].f_redir_output)
			*f_output = 1;
		n->i++;
	}
}

void	handle_pipes(t_data **d, t_ints *n)
{
	int	f_input;
	int	f_output;

	check_for_redirs(d, n, &f_input, &f_output);
	if (n->j == 0)
	{
		if (f_output == 0)
			dup2((*d)->fd[1], STDOUT_FILENO);
	}
	else if (n->j > 0 && n->j < (*d)->num_commands - 1)
	{
		if (f_output == 0)
			dup2((*d)->fd[1], STDOUT_FILENO);
		if (f_input == 0)
			dup2((*d)->old_fd, STDIN_FILENO);
	}
	else if (n->j == (*d)->num_commands - 1)
	{
		if (f_input == 0)
			dup2((*d)->old_fd, STDIN_FILENO);
	}
	close_pipe(d);
	n->i++;
}
