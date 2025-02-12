/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:17:53 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/12 13:18:10 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"

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
