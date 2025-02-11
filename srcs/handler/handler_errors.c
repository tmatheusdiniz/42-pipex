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

void	handle_errors(t_cmd **cmd, t_pipex **pipex, int index)
{
	int	i;
	(void)index;

	i = 0;
	if (cmd && *cmd)
	{
		if ((*cmd)->path)
			free((*cmd)->path);
	}
	if (pipex && *pipex)
	{
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
			free((*pipex)->pipe);
		}
	}
	free(*cmd);
	free(*pipex);
}
