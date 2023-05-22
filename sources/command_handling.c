/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:10:25 by francsan          #+#    #+#             */
/*   Updated: 2023/05/22 19:19:05 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	handle_builtin_cmd(t_data **d) // NEED TO FINISH ALL COMMANDS
{
	t_ints	n;
	char	*pwd;
	char	*path;

	n.i = 0;
	if ((*d)->tokens[n.i].f_builtins[1] == 1)
	{
		if (ft_strncmp((*d)->tokens[n.i + 1].token, "..", 2) == 0)
		{
			n.k = get_pwd(d, &pwd);
			// printf("\n\nPWD: %s\n\n", pwd); // TESTING
			n.j = ft_strlen(pwd);
			while (--n.j >= 0)
				if (pwd[n.j] == '/')
					break ;
			path = ft_calloc(n.j + 1, sizeof(char));
			ft_strlcpy(path, pwd, n.j + 1);
			// printf("\n\nPath: %s\n\n", path); // TESTING
			chdir(path);
			free((*d)->env[n.k]);
			(*d)->env[n.k] = ft_strdup(path);
			free(path);
			free(pwd);
		}
		else
		{
			chdir((*d)->tokens[n.i + 1].token);
		}
	}
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
		check_redir(d, n->l);
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
