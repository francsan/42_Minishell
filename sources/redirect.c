#include "../headers/minishell.h"

t_red	**red_func(t_red *red)
{
	static t_red	*io;

	if (red)
		io = red;
	return (&io);
}

int	check_io_dup(t_red *red)
{
	t_red *tmp;

	tmp = red;
	tmp = tmp->next;
	while (tmp)
	{
		if (red->output == tmp->output)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int treat_input(t_red *red, t_cmd *cmd)
{
	if (red->is_two)
		red->fd = open(red->file, O_RDONLY);
	if (!red->is_two&& red->fd == -1)
	{
		//error;
		return (1);
	}
	if (check_io_dup(red))
	{
		close(red->fd);
		return (0);
	}
	if (cmd->infd != -1 && cmd->infd != 0)
		close(cmd->infd);
	cmd->infd = red->fd;
	return (0);
}

int treat_output(t_red *red, t_cmd *cmd)
{
	if (red->is_two)
		red->fd = open(red->file, O_CREAT | O_APPEND | O_WRONLY, 0644);
	else
		red->fd = open(red->file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (red->fd == -1)
	{
		//error;
		return (1);
	}
	if (check_io_dup(red))
	{
		if (cmd->outfd != -1 && cmd->outfd != 0)
			close(red->fd);
		cmd->outfd = red->fd;
	}
	else
		close(red->fd);
	return (0);
}

void	rem_ref(t_red *rem)
{
	t_red	**beg;
	t_red	*tmp;

	beg = red_func(NULL);
	tmp = *beg;
	if (!tmp || !rem)
		return ;
	if (*beg == rem)
		*beg = (*beg)->next;
	else
	{
		tmp = *beg;
		while (tmp->next && rem != tmp->next)
			tmp = tmp->next;
		tmp->next = rem->next;
	}
	if (rem->file)
		free(rem->file);
	free(rem);
}

int redir_check(t_red **red, t_cmd *cmd)
{
	while (*red_func(NULL))
	{
		if ((*red)->output)
			if (treat_output(*red, cmd))
				return (1);
		if (!(*red)->output)
			if (treat_input(*red, cmd))
				return (1);
		rem_ref(*red);
	}
	return (0);
}

int redir_prep(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_red	*head;

	tmp = cmd;
	//if (heredoc(cmd))
	//	return (1);
	tmp = cmd;	
	while (tmp)
	{
		head = tmp->io;
		red_func(head);
		if (redir_check(red_func(NULL), tmp))
			return (1);
		tmp->io = *red_func(NULL);
		tmp = tmp->next;
	}
	return (0);
}