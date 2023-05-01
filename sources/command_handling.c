/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:10:25 by francsan          #+#    #+#             */
/*   Updated: 2023/05/01 21:26:14 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	**get_cmd(t_data **d, t_iterators *n)
{
	t_iterators	m;
	char		**cmd;

	m.i = n->i;
	while ((*d)->tokens[m.i].token && (*d)->tokens[m.i].f_pipe == 0)
		m.i++;
	cmd = ft_calloc(m.i + 1, sizeof(char *));
	m.j = 0;
	while ((*d)->tokens[n->i].token && (*d)->tokens[n->i].f_pipe == 0)
	{
		cmd[m.j] = ft_strdup((*d)->tokens[n->i].token);
		n->i++;
		m.j++;
	}
	print_array(cmd); // TESTING
	return (cmd);
}

void	handle_command(t_data **d)
{
	t_iterators	n;
	pid_t		pid;
	char		**cmd;

	n.i = 0;
	cmd = get_cmd(d, &n);
	pid = fork();
	if (pid < 0)
		return ;
	else if (pid == 0)
	{
		execve(cmd[0], cmd, (*d)->env);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	ft_strarr_free(cmd);
}

void	handle_pipes(t_data **d)
{
	t_iterators	n;
	pid_t		pid;
	char		***cmds;

	n.i = 0;
	n.j = 0;
	cmds = ft_calloc((*d)->num_commands + 1, sizeof(char **));
	while (n.j < (*d)->num_commands)
	{
		cmds[n.j] = get_cmd(d, &n);
		printf("\n\nAntes: %d\n\n", n.j);
		n.j++;
		printf("\n\nDepois: %d\n\n", n.j);
	}
	n.j = 0;
	// print_array(cmds[0]); // TESTING
	while (n.j < (*d)->num_commands)
	{
		pid = fork();
		if (pid < 0)
			return ;
		else if (pid == 0)
		{
			execve(cmds[n.j][0], cmds[n.j], (*d)->env);
			exit(0);
		}
		n.j++;
	}
	waitpid(-1, NULL, 0);
	n.k = -1;
	while (cmds[++n.k])
		ft_strarr_free(cmds[n.k]);
	free(cmds);
}
