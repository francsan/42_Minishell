/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:52:08 by francsan          #+#    #+#             */
/*   Updated: 2023/07/18 18:48:09 by francsan         ###   ########.fr       */
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
		// printf("%s%s%s\n", BLUE, tokens[i], DEFAULT);
		problem_solver(env[j], &temp);
		// printf("%s%s%s\n", GREEN, temp, DEFAULT);
		if (ft_strlen(tokens[i]) >= ft_strlen(temp))
		{
			if (ft_strncmp(tokens[i], temp, ft_strlen(tokens[i])) == 0)
			{
				free(temp);
				return (1);
			}
		}
		else
		{
			if (ft_strncmp(tokens[i], temp, ft_strlen(temp)) == 0)
			{
				free(temp);
				return (1);
			}
		}
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
			n.j = check_var_exist(tokens[n.i], env->env);
			if (n.j != -1 && tokens[n.i][strrlen(tokens[n.i], '=') - 1] == '=')
			{
				free(env->env[n.j]);
				env->env[n.j] = ft_strdup(tokens[n.i]);
			}
			else if (n.j == -1)
			{
				if (handle_new_var(tokens, env_func(), n.i))
				{
					g_exitvalue = 255;
					return (1);
				}
			}
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
