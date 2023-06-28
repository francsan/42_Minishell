/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:52:08 by francsan          #+#    #+#             */
/*   Updated: 2023/06/28 16:52:26 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	built_check_unset(char **tokens, char **env, int i)
{
	int	j;

	j = -1;
	while (env[++j])
		if (!ft_strncmp(tokens[i], env[j], \
		(strrlen(tokens[i], -1) - 1)))
			return (1);
	return (0);
}

int	built_unset(char **tokens, t_env *env)
{
	int	i;

	i = -1;
	while (tokens[++i])
	{
		if (built_check_unset(tokens, env->env, i))
		{
			env->status = env_set(tokens, env, i);
			if (env->status)
				return (1);
		}
	}
	return (0);
}

int	built_export(char **tokens, t_env *env, int outfd)
{
	int	i;
	int	j;

	i = 0;
	if (!tokens[1])
		export_print(env->env, outfd);
	while (tokens[++i])
	{
		if (is_valid(tokens[i]))
		{
			j = var_pos(tokens[i], env->env);
			if (j != -1 && tokens[i][strrlen(tokens[i], '=') - 1] == '=')
			{
				free(env->env[j]);
				env->env[j] = ft_strdup(tokens[i]);
			}
			else if (j == -1)
				if (handle_no_var(tokens, env_func(), i))
					return (225);
		}
		else
			return (0); //print_error("not a valid indentifier")
	}
	return (0);
}
