/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:36:59 by francsan          #+#    #+#             */
/*   Updated: 2023/06/27 19:47:45 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	is_num(char *str)
{
	int	i;

	i = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] < '0' && str[i] > '9')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i])
		return (0);
	return (1);
}

void	export_print(char **env, int out)
{
	int	i;
	int	len;

	i = -1;
	while (env[++i])
	{
		len = strrlen(env[i], '=');
		write(out, "declare -x ", 11);
		write(out, env[i], len);
		if (env[i][len - 1] != '=')
			write(out, "=", 1);
		write(out, "\"", 1);
		if (env[i][len])
			write(out, env[i] + len, strrlen(env[i] + len, -1));
		write(out, "\"\n", 2);
	}
}

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
