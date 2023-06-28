/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 00:26:04 by francisco         #+#    #+#             */
/*   Updated: 2023/06/29 00:29:04 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	var_pos(char *str, char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		if (!ft_strncmp(str, env[i], strrlen(env[i], '=') - 1))
			return (i);
	return (-1);
}

int	ft_len(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		;
	return (i);
}

int	handle_no_var(char **tokens, t_env *env, int i)
{
	char	**tmp;
	int		j;

	j = -1;
	tmp = env->env;
	env->env = ft_calloc((ft_len(env->env) + 2), sizeof(char *));
	if (!env->env)
	{
		env->env = tmp;
		return (-1);
	}
	while (tmp[++j])
		env->env[j] = tmp[j];
	free(tmp);
	env->env[j] = ft_strdup(tokens[i]);
	return (0);
}
