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

static void	init_all(t_cmd **cmd, t_pipex **pipex, int count_arg);
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

static void	init_all(t_cmd **cmd, t_pipex **pipex, int count_arg)
{
	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	*pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!*cmd || !*pipex)
		handle_errors(cmd, pipex, 1);
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
}

int	main(int count_arg, char **str, char **env_var)
{
	t_cmd	*cmd;
	t_pipex	*pipex;
	init_all(&cmd, &pipex, count_arg);	
	if (open_files(&pipex, str[1], str[count_arg - 1]) == 4)
		handle_errors(&cmd, &pipex, 4);
	handle_forks(str, &cmd, &pipex, env_var);
}
