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

static int	aux_handle(t_pipex **pipex, int *i);

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

static int	aux_handle(t_pipex **pipex, int *i)
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

void	handle_forks(char **str, t_cmd **cmd, t_pipex **pipex, char **env_var)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
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
			j = find_bin(cmd, (*cmd)->commands[0], env_var);
			if (j == 1 || j == 2)
			{
				matrix_cleanup((*cmd)->commands);
				handle_errors(cmd, pipex, j);
			}
			if (aux_handle(pipex, &i) == 5)
			{
				matrix_cleanup((*cmd)->commands);
				handle_errors(cmd, pipex, 5);
			}
			execve((*cmd)->path, (*cmd)->commands, env_var);
			handle_errors(cmd, pipex, 6);
		}
		i ++;
	}
	close_pipes(pipex);
	i = 0;
	while (i < (*pipex)->number_commands)
		waitpid((*cmd)->pid[i++], NULL, 0);
}

int	open_files(t_pipex **pipex, char *infile, char *outfile)
{
	int	fd;

	fd = open(infile, O_RDWR);
	if (fd < 0)
		return (4);
	(*pipex)->fd_in = fd;
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (close((*pipex)->fd_in), 4);
	(*pipex)->fd_out = fd;
	return (0);
}
