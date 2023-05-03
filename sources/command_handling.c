/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:10:25 by francsan          #+#    #+#             */
/*   Updated: 2023/05/03 21:12:44 by francsan         ###   ########.fr       */
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

void	close_pipe(t_data **d)
{
	close((*d)->pipe[0]);
	close((*d)->pipe[1]);
}

void	handle_pipes(t_data **d, t_ints *n)
{
	while ((*d)->tokens[n->i].token && (*d)->tokens[n->i].f_pipe == 0)
	{
		if ((*d)->tokens[n->i].f_redir_input == 0 \
			&& (*d)->tokens[n->i].f_redir_output == 0)
			n->i++;
		else if((*d)->tokens[n->i].f_redir_input == 1 \
			|| (*d)->tokens[n->i].f_redir_output == 1 \
			|| (*d)->tokens[n->i + 1].f_pipe == 1)
		{
			if ((*d)->tokens[n->i].f_redir_input == 1)
			{
				// dup2(infile, STDIN_FILENO); // INFILE
			}
			else if (n->j != 0)
			{
				dup2((*d)->old_pipe, STDIN_FILENO);
			}
			else if (n->j == 0)
				dup2((*d)->pipe[1], STDOUT_FILENO);
			if ((*d)->tokens[n->i].f_redir_output == 1)
			{
				// dup2(outfile, STDOUT_FILENO); // OUTFILE
			}
			else if (n->j != (*d)->num_commands - 1)
			{
				dup2((*d)->pipe[1], STDOUT_FILENO);
			}
			n->i++;
		}
	}
	close_pipe(d);
	n->i++;
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
	(*d)->pid = ft_calloc((*d)->num_commands + 1, sizeof(pid_t));
	n.i = 0;
	n.j = 0;
	while (n.j < (*d)->num_commands)
	{
		if (pipe((*d)->pipe) < 0)
			return ;
		(*d)->pid[n.j] = fork();
		if ((*d)->pid[n.j] < 0)
			return ;
		else if ((*d)->pid[n.j] == 0)
		{
			handle_pipes(d, &n);
			execve(cmds[n.j][0], cmds[n.j], (*d)->env);
			exit(0);
		}
		(*d)->old_pipe = dup((*d)->pipe[0]);
		close_pipe(d);
		waitpid((*d)->pid[n.j], NULL, 0);
		n.j++;
	}
	free((*d)->pid);
	n.k = -1;
	while (cmds[++n.k])
		ft_strarr_free(cmds[n.k]);
	free(cmds);
}
