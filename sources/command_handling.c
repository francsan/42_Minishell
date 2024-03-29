/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:10:25 by francsan          #+#    #+#             */
/*   Updated: 2023/08/08 17:40:41 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_cmd	*add_cmd(t_cmd *cmd)
{
	t_cmd	*new;
	t_cmd	*tmp;

	tmp = NULL;
	new = ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->infd = 0;
	new->outfd = 1;
	new->next = NULL;
	if (!cmd)
	{
		cmd = new;
		return (new);
	}
	tmp = cmd;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (new);
}

void	handle_builtin_cmd(t_data **d, char ***tokens)
{
	t_cmd	*comando;

	comando = add_cmd(NULL);
	exec_builtin(d, tokens, comando->outfd);
	free(comando);
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
		check_redir(d, 0);
		if (execve(cmd[0], cmd, env_func()->env) == -1)
			perror(cmd[0]);
		exit(0);
	}
	waitpid(pid, &g_exitvalue, 0);
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
		if (access(cmds[n->j][0], F_OK) == -1)
		{
			usleep (5000);
			cmd_not_found(d, cmds[n->j]);
			exit(0);
		}
		handle_pipes(d, n);
		check_redir(d, n->l);
		if (execve(cmds[n->j][0], cmds[n->j], env_func()->env) == -1)
			perror(cmds[n->j][0]);
		exit(0);
	}
	(*d)->old_fd = dup((*d)->fd[0]);
	close_pipe(d);
	n->j++;
}

void	handle_multiple_cmds(t_data **d)
{
	t_ints	n;
	char	***cmds;

	n.i = 0;
	n.j = 0;
	cmds = ft_calloc(((*d)->num_pipes + 1) + 1, sizeof(char **));
	while (n.j < ((*d)->num_pipes + 1))
	{
		cmds[n.j] = get_cmd(d, &n);
		n.i++;
		n.j++;
	}
	n.i = 0;
	n.j = 0;
	(*d)->pid = ft_calloc(((*d)->num_pipes + 1) + 1, sizeof(pid_t));
	while (n.j < ((*d)->num_pipes + 1))
		run_cmd(d, &n, cmds);
	n.j = 0;
	while (n.j < ((*d)->num_pipes + 1))
		waitpid((*d)->pid[n.j++], &g_exitvalue, 0);
	free((*d)->pid);
	n.k = -1;
	while (cmds[++n.k])
		ft_strarr_free(cmds[n.k]);
	free(cmds);
}
