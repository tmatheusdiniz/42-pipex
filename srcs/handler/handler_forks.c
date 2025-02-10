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

static int	aux_handle(char **str, t_cmd **cmd, t_pipex **pipex, int *i)
{
	if (*i == 0)
	{
		dup2((*pipex)->fd_in, STDIN_FILENO);
		dup2((*pipex)->pipe[*i][1], STDOUT_FILENO);
	}
	/*else (*i == (*pipex)->number_commands - 1)
	{
		
	}
	*/
}

int	open_files(t_pipex **pipex, char *infile, char *outfile)
{
	int	fd;

	fd = open(infile, O_RDWR);
	if (fd < 0)
		return (4);
	(*pipex)->fd_in = fd;
	fd = open(outfile, O_RDWR);
	if (fd < 0)
		return (4);
	(*pipex)->fd_out = fd;
}

void	handle_forks(char **str, t_cmd **cmd, t_pipex **pipex, char **env_var)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < (*pipex)->number_commands)
	{
		j = find_bin(cmd, str[i + 2], env_var);
		if (j == 1 || j == 2)
			handle_errors(cmd, pipex, j);
		(*cmd)->pid[i] = fork();
		if (!(*cmd)->pid[i])
			handle_errors(cmd, pipex, 3);
		if ((*cmd)->pid[i] == 0)
			break ;
		i ++;
	}
	
}
