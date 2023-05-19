/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 00:02:10 by francisco         #+#    #+#             */
/*   Updated: 2023/05/19 13:28:06 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*skip_quotes(t_data **d, t_ints *n)
{
	char	*redir;

	n->j = 0;
	while ((*d)->tokens[n->i].token[n->j] != '"' && (*d)->tokens[n->i].token[n->j] != '\'')
		n->j++;
	if ((*d)->tokens[n->i].token[n->j] == '"')
	{
		n->j++;
		while ((*d)->tokens[n->i].token[n->j] != '"')
			n->j++;
		n->j++;
	}
	else if ((*d)->tokens[n->i].token[n->j] == '\'')
	{
		n->j++;
		while ((*d)->tokens[n->i].token[n->j] != '\'')
			n->j++;
		n->j++;
	}
	redir = ft_strdup(&(*d)->tokens[n->i].token[n->j]);
	return (redir);
}

void	handle_redirections(t_data **d, t_ints *n, char *redir)
{
	if (redir[0] == '<')
	{
		if (redir[1] == '\0')
			(*d)->infile = open((*d)->tokens[n->i + 1].token, O_RDONLY);
		else if (redir[1] == '<')
		{
			if (redir[2] == '\0')
				(*d)->infile = open((*d)->tokens[n->i + 1].token, O_RDONLY);
			else if (redir[2])
				(*d)->infile = open(&redir[2], O_RDONLY);
		}
		else if (redir[1] != '<' && redir[1])
			(*d)->infile = open(&redir[1], O_RDONLY);
		dup2((*d)->infile, STDIN_FILENO);
		close((*d)->infile);
	}
	else if (redir[0] == '>')
	{
		if (redir[1] == '\0')
			(*d)->outfile = open((*d)->tokens[n->i + 1].token, O_CREAT | O_TRUNC | O_WRONLY, 0666);
		else if (redir[1] == '>')
		{
			if (redir[2] == '\0')
				(*d)->outfile = open((*d)->tokens[n->i + 1].token, O_CREAT | O_APPEND | O_WRONLY, 0666);
			else if (redir[2])
				(*d)->outfile = open(&redir[2], O_CREAT | O_APPEND | O_WRONLY, 0666);
		}
		else if (redir[1] != '>' && redir[1])
			(*d)->outfile = open(&redir[1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
		dup2((*d)->outfile, STDOUT_FILENO);
		close((*d)->outfile);
	}
}

void	check_redir(t_data **d, int cmd_num)
{
	t_ints	n;
	char	*redir;

	n.i = cmd_num;
	if ((*d)->tokens[n.i].f_pipe == 1)
		n.i++;
	while ((*d)->tokens[n.i].token)
	{
		if ((*d)->tokens[n.i].f_redir_input == 1 || (*d)->tokens[n.i].f_redir_output == 1)
		{
			if ((*d)->tokens[n.i].f_singlequotes == 1 || (*d)->tokens[n.i].f_doublequotes == 1)
			{
				redir = skip_quotes(d, &n);
				handle_redirections(d, &n, redir);
				free(redir);
			}
			else
				handle_redirections(d, &n, (*d)->tokens[n.i].token);
		}
		n.i++;
	}
}