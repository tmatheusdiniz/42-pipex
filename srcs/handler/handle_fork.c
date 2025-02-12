/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_forks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:09:33 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/10 22:51:15 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"
#include <fcntl.h>
#include <unistd.h>

static int	aux_handle_dup(t_pipex **pipex, int *i);
static void	aux_handle_parent(t_pipex **pipex, t_cmd **cmd);
static void	aux_handle_forks(t_cmd **cmd, t_pipex **pipex,
				char **env_var, int *i);

static void	close_pipes(t_pipex **pipex)
{
	int	i;

	i = 0;
	while (i < (*pipex)->number_commands - 1)
	{
		close((*pipex)->pipe[i][0]);
		(*pipex)->pipe[i][0] = -1;
		close((*pipex)->pipe[i][1]);
		(*pipex)->pipe[i][1] = -1;
		i++;
	}
}

static int	aux_handle_dup(t_pipex **pipex, int *i)
{
	if (*i == 0)
	{
		close((*pipex)->pipe[*i][0]);
		if (dup2((*pipex)->fd_in, STDIN_FILENO) == -1)
			return (5);
		if (dup2((*pipex)->pipe[*i][1], STDOUT_FILENO) == -1)
			return (5);
	}
	else if (*i > 0 && *i < (*pipex)->number_commands - 1)
	{
		if (dup2((*pipex)->pipe[*i - 1][0], STDIN_FILENO) == -1)
			return (5);
		if (dup2((*pipex)->pipe[*i][1], STDOUT_FILENO) == -1)
			return (5);
	}
	else
	{
		if (dup2((*pipex)->pipe[*i - 1][0], STDIN_FILENO) == -1)
			return (5);
		if (dup2((*pipex)->fd_out, STDOUT_FILENO) == -1)
			return (5);
	}
	close_pipes(pipex);
	return (0);
}

static void	aux_handle_parent(t_pipex **pipex, t_cmd **cmd)
{
	int	i;

	i = 0;
	close_pipes(pipex);
	while (i < (*pipex)->number_commands)
		waitpid((*cmd)->pid[i++], NULL, 0);
}

static void	aux_handle_forks(t_cmd **cmd,
		t_pipex **pipex, char **env_var, int *i)
{
	int	j;

	j = 0;
	j = find_bin(cmd, (*cmd)->commands[0], env_var);
	if (j == 1 || j == 2)
	{
		matrix_cleanup((*cmd)->commands);
		handle_errors(cmd, pipex, j);
	}
	else if (j == -2)
		handle_errors(cmd, pipex, j);
	if (aux_handle_dup(pipex, i) == 5)
		handle_errors(cmd, pipex, 5);
	execve((*cmd)->path, (*cmd)->commands, env_var);
	handle_errors(cmd, pipex, 6);
}

void	handle_forks(char **str, t_cmd **cmd, t_pipex **pipex, char **env_var)
{
	int	i;

	i = 0;
	while (i < (*pipex)->number_commands)
	{
		(*cmd)->pid[i] = fork();
		if ((*cmd)->pid[i] < 0)
			handle_errors(cmd, pipex, 3);
		if ((*cmd)->pid[i] == 0)
		{
			(*cmd)->commands = ft_split(str[i + 2], ' ');
			if (!(*cmd)->commands)
				handle_errors(cmd, pipex, 1);
			aux_handle_forks(cmd, pipex, env_var, &i);
		}
		i ++;
	}
	aux_handle_parent(pipex, cmd);
}
