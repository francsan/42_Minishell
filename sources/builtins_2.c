/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:52:08 by francsan          #+#    #+#             */
/*   Updated: 2023/06/15 18:52:10 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	check_unset(t_cmd *cmd, char **env, int i)
{
	int	j;

	j = -1;
	while (env[++j])
		if (!ft_strncmp(cmd->args[i], env[j], \
		(ft_strlen(cmd->args[i]) - 1)))
			return (1);
	return (0);
}

int	unset(t_cmd *cmd, t_env *env)
{
	int	i;

	i = -1;
	while (cmd->args[++i])
	{
		if (check_unset(cmd, env->env, i))
		{
			env->status = env_set(cmd, env, i);
			if (env->status)
				return (1);
		}
	}
	return (0);
}

int	cd(char **tokens, t_env *env)
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

int	export(t_cmd *cmd, t_env *env, int outfd)
{
	int	i;
	int	j;

	i = 0;
	if (!cmd->args[1])
		export_print(env->env, outfd);
	while (cmd->args[++i])
	{
		if (is_valid(cmd->args[i]))
		{
			j = var_pos(cmd->args[i], env->env);
			if (j != -1 && cmd->args[i][strrlen(cmd->args[i], '=') - 1] == '=')
			{
				free(env->env[j]);
				env->env[j] = ft_strdup(cmd->args[i]);
			}
			else if (j == -1)
				if (handle_no_var(cmd, env_func(), i))
					return(225);
		}
		else
			return (0); //print_error("not a valid indentifier")
	}
	return (0);
}

int	exit_builtin(char **tokens, t_env *env)
{
	unsigned char status;

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