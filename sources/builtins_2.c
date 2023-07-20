/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:52:08 by francsan          #+#    #+#             */
/*   Updated: 2023/07/20 16:59:24 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	built_check_unset(char **tokens, char **env, int i)
{
	int		j;
	char	*temp;

	j = -1;
	while (env[++j])
	{
		problem_solver(env[j], &temp);
		if (check_unset_var_name(tokens, &temp, i))
			return (1);
		free(temp);
	}
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
	t_ints	n;

	n.i = 0;
	if (!tokens[1])
		export_print(env->env, outfd);
	while (tokens[++n.i])
	{
		if (is_valid(tokens[n.i]))
		{
			if (export_var(&n, env, tokens))
				return (1);
		}
		else
		{
			g_exitvalue = 1;
			printf("export: %s: not a valid identifier\n", tokens[1]);
			return (0);
		}
	}
	return (0);
}
