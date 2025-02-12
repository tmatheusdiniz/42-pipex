/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:59:19 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/10 16:44:58 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"
#include <fcntl.h>
#include <stdlib.h>

static void	init_all(t_cmd **cmd, t_pipex **pipex, char **str, int count_arg);
static int	create_pipe(t_pipex **pipex);

static void	pipe_cleanup(t_pipex **pipex, int index)
{
	int	i;

	i = 0;
	while (i < index)
		free((*pipex)->pipe[i++]);
	free ((*pipex)->pipe);
}

static int	create_pipe(t_pipex **pipex)
{
	int	i;

	i = 0;
	while (i < (*pipex)->number_commands - 1)
	{
		(*pipex)->pipe[i] = (int *)malloc(sizeof(int) * 2);
		if (!(*pipex)->pipe[i])
			return (pipe_cleanup(pipex, i), 1);
		if (pipe((*pipex)->pipe[i]) < 0)
			return (pipe_cleanup(pipex, i), 1);
		i ++;
	}
	return (0);
}

static void	init_all(t_cmd **cmd, t_pipex **pipex, char **str, int count_arg)
{
	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	*pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!*cmd || !*pipex)
		handle_errors(cmd, pipex, 1);
	(*cmd)->path = NULL;
	(*cmd)->commands = NULL;
	(*cmd)->pid = NULL;
	(*pipex)->fd_in = 0;
	(*pipex)->fd_out = 0;
	(*pipex)->number_commands = count_arg - 3;
	(*pipex)->pipe = (int **)malloc(sizeof(int *)
			* ((*pipex)->number_commands + 1));
	if (!(*pipex)->pipe)
		handle_errors(cmd, pipex, 1);
	if (create_pipe(pipex) == 1)
		handle_errors(cmd, pipex, 1);
	(*cmd)->pid = (pid_t *)malloc(sizeof(pid_t) * (*pipex)->number_commands);
	if (!(*cmd)->pid)
		handle_errors(cmd, pipex, 1);
	(*cmd)->infile = str[1];
	(*cmd)->outfile = str[count_arg - 1];
}

int	main(int count_arg, char **str, char **env_var)
{
	t_cmd	*cmd;
	t_pipex	*pipex;

	if (count_arg < 5)
	{
		ft_printf("You should send least 5 paremeters!\n");
		exit (0);
	}
	init_all(&cmd, &pipex, str, count_arg);
	if (open_files(&pipex, cmd->infile, cmd->outfile) == 4)
		handle_errors(&cmd, &pipex, 4);
	handle_forks(str, &cmd, &pipex, env_var);
	clean_cmd(&cmd);
	clean_pipex(&pipex);
}
