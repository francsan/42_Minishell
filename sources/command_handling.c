/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:10:25 by francsan          #+#    #+#             */
/*   Updated: 2023/04/27 16:59:55 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	**get_cmd(t_data **d)
{
	t_iterators	n;
	char		**cmd;

	n.i = 0;
	while ((*d)->tokens[n.i].token && (*d)->tokens[n.i].f_pipe == 0)
		n.i++;
	cmd = ft_calloc(n.i + 1, sizeof(char *));
	n.i = 0;
	while ((*d)->tokens[n.i].token && (*d)->tokens[n.i].f_pipe == 0)
	{
		cmd[n.i] = ft_strdup((*d)->tokens[n.i].token);
		n.i++;
	}
	return (cmd);
}

void	handle_command(t_data **d)
{
	pid_t	pid;
	char	**cmd;

	cmd = get_cmd(d);
	pid = fork();
	if (pid < 0)
		return ;
	else if (pid == 0)
	{
		execve(cmd[0], cmd, (*d)->env);
		exit(0);
	}
	waitpid(pid, NULL, 0);
}
