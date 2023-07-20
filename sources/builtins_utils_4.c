/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:49:06 by francsan          #+#    #+#             */
/*   Updated: 2023/07/20 16:59:28 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	check_unset_var_name(char **tokens, char **temp, int i)
{
	if (ft_strlen(tokens[i]) >= ft_strlen(*temp))
	{
		if (ft_strncmp(tokens[i], *temp, ft_strlen(tokens[i])) == 0)
		{
			free(*temp);
			return (1);
		}
	}
	else
	{
		if (ft_strncmp(tokens[i], *temp, ft_strlen(*temp)) == 0)
		{
			free(*temp);
			return (1);
		}
	}
	return (0);
}

int	export_var(t_ints *n, t_env *env, char **tokens)
{
	n->j = check_var_exist(tokens[n->i], env->env);
	if (n->j != -1 && tokens[n->i][strrlen(tokens[n->i], '=') - 1] == '=')
	{
		free(env->env[n->j]);
		env->env[n->j] = ft_strdup(tokens[n->i]);
	}
	else if (n->j == -1)
	{
		if (handle_new_var(tokens, env_func(), n->i))
		{
			g_exitvalue = 255;
			return (1);
		}
	}
	return (0);
}
