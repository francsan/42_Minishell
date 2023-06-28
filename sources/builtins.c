/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:51:46 by francsan          #+#    #+#             */
/*   Updated: 2023/06/28 16:50:02 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	env_change(t_env *env)
{
	char	*tmp;
	int		i;

	i = -1;
	while (env->env[++i])
		if (!ft_strncmp(env->env[i], "PWD=", 4))
			break ;
	tmp = ft_strjoin("OLD", env->env[i]);
	free(env->env[i]);
	env->env[i] = tmp;
	i = -1;
	while (env->env[++i])
		if (!strncmp(env->env[i], "OLDPWD=", 7))
			break ;
	tmp = getcwd(NULL, 0);
	free(env->env[i]);
	env->env[i] = ft_strjoin("PWD=", tmp);
	free(tmp);
	return (0);
}

int	built_cd(char **tokens, t_env *env)
{
	if (!tokens[1])
		if (var_set(tokens))
			return (1);
	if (tokens[2])
	{
		//error too many args
		return (1);
	}
	if (chdir(tokens[1]))
	{
		//error
		return (1);
	}
	if (env_change(env))
		return (1);
	return (0);
}

int	built_exit(char **tokens, t_env *env)
{
	unsigned char	status;

	printf("exit\n");
	if (tokens[1] && !is_num(tokens[1]))
	{
		status = 2;
		//print_error("numeric argument required\n");
	}
	else if (tokens[1] && tokens[2])
	{
		status = 1;
		//print_error("too many args");
		return (status);
	}
	else if (tokens[1])
		status = ft_atoi(tokens[1]);
	else
		status = env->status;
	free(tokens);
	if (env->env)
		free (env->env);
	rl_clear_history();
	exit(status);
}

int	exec_builtin(char **tokens, int outfd)
{
	if (!ft_strncmp(*tokens, "cd", 3))
		return (built_cd(tokens, env_func()));
	else if (!strncmp(*tokens, "unset", 6))
		return (built_unset(tokens, env_func()));
	else if (!strncmp(*tokens, "export", 7))
		return (built_export(tokens, env_func(), outfd));
	else if (!strncmp(*tokens, "exit", 5))
		return (built_exit(tokens, env_func()));
	else
		return (0);
}
