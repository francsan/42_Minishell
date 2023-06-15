/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:51:46 by francsan          #+#    #+#             */
/*   Updated: 2023/06/15 20:27:12 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int pwd(void)//int out
{
	char *pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		//error;
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

int	env(char **envp) //t_cmd *cmd, int out
{
	int i;

	i = -1;
	if (!envp)
		printf("Env don't exist.\n");
	// while (cmd->args[i++])
	// 	printf("chegou\n");
	// if (i > 1)
	// 	return (1);
	i = -1;
	while (envp[++i])
	{
		if (strchr(envp[i], '='))
			printf("%s\n", envp[i]);
	}
	return (0);
}

int	echo(char **tokens)
{
	int	i;

	i = 0 + ft_strncmp(tokens[1], "-n", 3);
	while (tokens[++i])
	{
		printf("%s", tokens[1]);
		if (tokens[i + 1])
			printf(" ");
	}
	if (ft_strncmp(tokens[1], "-n", 3))
		printf("\n");
	return (0);
}

int	exec_builtin(t_cmd *cmd, char **tokens, int outfd)
{
	if(!ft_strncmp(*tokens, "cd", 3))
		return (cd(tokens, env_func()));
	else if(!strncmp(*tokens, "pwd", 3))
		return (pwd());
	else if(!strncmp(*tokens, "env", 4))
		return (env(env_func()->env));//cmd, outfd 
	else if(!strncmp(*tokens, "echo", 5))
		return (echo(tokens));
	else if(!strncmp(*tokens, "unset", 6))
		return (unset(cmd, env_func()));
	else if(!strncmp(*tokens, "export", 7))
		return (export(tokens, env_func(), outfd));
	else if(!strncmp(*tokens, "exit", 5))
		return (exit_builtin(tokens, env_func()));
	else
		return (0);
}
