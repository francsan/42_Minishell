/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:10:25 by francsan          #+#    #+#             */
/*   Updated: 2023/05/05 13:54:50 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	**get_cmd(t_data **d, t_ints *n)
{
	t_ints	m;
	char	**cmd;

	m.i = n->i;
	while ((*d)->tokens[m.i].token && (*d)->tokens[m.i].f_pipe == 0)
		m.i++;
	cmd = ft_calloc(m.i - n->i + 1, sizeof(char *));
	m.j = 0;
	while ((*d)->tokens[n->i].token && (*d)->tokens[n->i].f_pipe == 0)
	{
		cmd[m.j] = ft_strdup((*d)->tokens[n->i].token);
		n->i++;
		m.j++;
	}
	return (cmd);
}

void	handle_single_cmd(t_data **d)
{
	t_ints	n;
	pid_t	pid;
	char	**cmd;

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

void	run_cmd(t_data **d, t_ints *n, char ***cmds)
{
	if (pipe((*d)->fd) == -1)
		return ;
	(*d)->pid[n->j] = fork();
	if ((*d)->pid[n->j] < 0)
		return ;
	else if ((*d)->pid[n->j] == 0)
	{
		handle_pipes(d, n);
		execve(cmds[n->j][0], cmds[n->j], (*d)->env);
		exit(0);
	}
	(*d)->old_fd = dup((*d)->fd[0]);
	close_pipe(d);
	waitpid((*d)->pid[n->j], NULL, 0);
	n->j++;
}

void	handle_multiple_cmds(t_data **d)
{
	t_ints	n;
	char	***cmds;

	n.i = 0;
	n.j = 0;
	cmds = ft_calloc((*d)->num_commands + 1, sizeof(char **));
	while (n.j < (*d)->num_commands)
	{
		cmds[n.j] = get_cmd(d, &n);
		n.i++;
		n.j++;
	}
	n.i = 0;
	n.j = 0;
	(*d)->pid = ft_calloc((*d)->num_commands + 1, sizeof(pid_t));
	while (n.j < (*d)->num_commands)
		run_cmd(d, &n, cmds);
	free((*d)->pid);
	n.k = -1;
	while (cmds[++n.k])
		ft_strarr_free(cmds[n.k]);
	free(cmds);
}
