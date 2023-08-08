/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:36:59 by francsan          #+#    #+#             */
/*   Updated: 2023/08/08 16:00:27 by francsan         ###   ########.fr       */
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

void	sort_env(char **env, int i, int j)
{
	char	*tmp;

	if (ft_strncmp(env[i], env[j], ft_strlen(env[i])) <= 0)
	{
		tmp = env[i];
		env[i] = env[j];
		env[j] = tmp;
	}
}

void	print_export(char **env, int out, int *i)
{
	int	len;

	len = strrlen(env[*i], '=');
	write(out, "declare -x ", 11);
	write(out, env[*i], len);
	if (env[*i][len - 1] == '=')
	{
		write(out, "\"", 1);
		if (env[*i][len])
			write(out, env[*i] + len, strrlen(env[*i] + len, -1));
		write(out, "\"\n", 2);
	}
	else
		write(out, "\n", 1);
}

void	export_print(char **env, int out)
{
	int		i;
	int		j;
	char	**temp;

	temp = ft_strarr_copy(env);
	i = -1;
	while (temp[++i])
	{
		j = -1;
		while (temp[++j])
			sort_env(temp, i, j);
	}
	i = -1;
	while (temp[++i])
	{
		if ((temp[i][0] >= 'A' && temp[i][0] <= 'Z') \
			|| (temp[i][0] >= 'a' && temp[i][0] <= 'z') \
			|| temp[i][0] == '\0')
			print_export(temp, out, &i);
	}
	ft_strarr_free(temp);
}
