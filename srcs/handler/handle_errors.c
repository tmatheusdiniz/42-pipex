/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:04:36 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/10 17:14:07 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"

static void	aux_open_files(t_cmd **cmd)
{
	if (errno == EACCES)
		ft_printf("pipex: permission denied: %s\n", (*cmd)->infile);
	else if (errno == ENOENT)
		ft_printf("pipex: no such file or directory: %s\n", (*cmd)->infile);
	else
		ft_printf("%s: %s\n",(*cmd)->infile, strerror(errno));
}

void	clean_cmd(t_cmd **cmd)
{
	int	i;

	i = 0;
	if ((*cmd)->path)
		free((*cmd)->path);
	if ((*cmd)->commands)
	{
		while ((*cmd)->commands[i])
			free ((*cmd)->commands[i++]);
		free ((*cmd)->commands);
	}
	if ((*cmd)->pid)
		free ((*cmd)->pid);
	free(*cmd);
}

void	clean_pipex(t_pipex **pipex)
{
	int	i;

	i = 0;
	if ((*pipex)->pipe)
	{
		while (i < (*pipex)->number_commands - 1)
		{
			if ((*pipex)->pipe[i])
			{
				if ((*pipex)->pipe[i][0] > 2)
					close((*pipex)->pipe[i][0]);
				if ((*pipex)->pipe[i][1] > 2)
					close((*pipex)->pipe[i][1]);
				free((*pipex)->pipe[i]);
			}
			i++;
		}
		free ((*pipex)->pipe);
	}
	if ((*pipex)->fd_in > 2)
		close((*pipex)->fd_in);
	if ((*pipex)->fd_out > 2)
		close((*pipex)->fd_out);
	free(*pipex);
}

void	handle_errors(t_cmd **cmd, t_pipex **pipex, int index)
{
	if (index == MALLOC_ERROR)
		ft_printf("pipex: malloc error!\n");
	else if (index == PATH_ERROR)
		ft_printf("pipex: couldn't find the path var in env_var!\n");
	else if (index == CMD_NOT_FOUND)
		ft_printf("pipex: command not found: %s\n", (*cmd)->commands[0]);
	else if (index == OPEN_FILE_ERROR)
		aux_open_files(cmd);
	else if (index == PROCESS_ERROR)
		ft_printf("pipex: error doing fork!\n");
	else if (index == DUP2_ERROR)
		ft_printf("pipex: error when dup2 is called!\n");
	else if (index == EXEC_ERROR)
		ft_printf("pipex: execve was not executed!\n");
	if (cmd && *cmd)
		clean_cmd(cmd);
	if (pipex && *pipex)
		clean_pipex(pipex);
	exit (1);
}
