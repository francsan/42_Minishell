/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 00:26:04 by francisco         #+#    #+#             */
/*   Updated: 2023/06/29 19:41:17 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	check_var_exist(char *str, char **env)
{
	int		i;
	char	*temp;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	temp = ft_calloc(i + 1, sizeof(char));
	i = 0;
	while (str[i] && str[i] != '=')
	{
		temp[i] = str[i];
		i++;
	}
	i = -1;
	while (env[++i])
		if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
			return (i);
	free(temp);
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

int	handle_new_var(char **tokens, t_env *env, int i)
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
