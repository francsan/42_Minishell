/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handling_utils_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francsan <francsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 00:05:43 by francisco         #+#    #+#             */
/*   Updated: 2023/08/29 18:35:23 by francsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	remove_quotes_2(t_data **d, t_ints *n, t_ints *m, char quotes)
{
	if ((*d)->tokens[n->i].token[m->i] == quotes)
	{
		m->i++;
	}
	else
	{
		if (m->l == 1)
			m->tmp[m->k] = (*d)->tokens[n->i].token[m->i];
		m->i++;
		m->k++;
	}
}

void	get_cmd_2(t_data **d, t_ints *n, t_ints *m, char ***cmd)
{
	if ((*d)->tokens[n->i].f_doublequotes == 1 \
		|| (*d)->tokens[n->i].f_singlequotes == 1)
	{
		if ((*d)->tokens[n->i].f_doublequotes == 1)
			(*cmd)[m->j] = remove_quotes(d, n, '"');
		else if ((*d)->tokens[n->i].f_singlequotes == 1)
			(*cmd)[m->j] = remove_quotes(d, n, '\'');
		n->i++;
		m->j++;
	}
	else if ((*d)->tokens[n->i].f_r_in == 1 \
		|| (*d)->tokens[n->i].f_r_out == 1)
		skip_redir(d, n);
	else
	{
		(*cmd)[m->j] = ft_strdup((*d)->tokens[n->i].token);
		n->i++;
		m->j++;
	}
}
