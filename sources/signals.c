/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 18:40:22 by francsan          #+#    #+#             */
/*   Updated: 2023/07/20 18:22:30 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	ignore_signal_i(void)
{
	signal(SIGINT, sig_handler_i);
	signal(SIGQUIT, sig_handler_i);
}

void	sig_handler_i(int sig)
{
	if (sig == SIGQUIT)
	{
		write(0, "Quit (core dumped)\n", 20);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	if (sig == SIGINT)
	{
		g_exitvalue = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	ignore_signal(void)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_handler(int sig)
{
	if (sig == SIGQUIT)
		return ;
	if (sig == SIGINT)
	{
		g_exitvalue = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return ;
}

void	if_ctrl_d(t_data **d, char *buffer, char *line, t_env *env)
{
	if (line)
		return ;
	rl_clear_history();
	write(1, "exit\n", 5);
	ft_strarr_free((*d)->paths);
	ft_strarr_free((*d)->env);
	ft_strarr_free(env->env);
	free(buffer);
	free(line);
	exit(g_exitvalue);
}
