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
#include <fcntl.h>

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

int	open_outfile_here(t_here_doc **here_doc)
{
	int	fd;

	fd = open((*here_doc)->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (4);
	(*here_doc)->outfile_fd = fd;
	fd = open((*here_doc)->save, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (close((*here_doc)->outfile_fd), 4);
	(*here_doc)->save_fd = fd;
	return (0);
}

int	restore_save(t_here_doc **here_doc)
{
	if ((*here_doc)->save_fd != -1)
		close ((*here_doc)->save_fd);
	(*here_doc)->save_fd = open ("here_doc_tmp", O_RDONLY);
	if ((*here_doc)->save_fd < 0)
		return (4);
	return (0);
}
