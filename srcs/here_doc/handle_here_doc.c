/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:09:24 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/12 22:33:55 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"

static void	init_structs_here_doc(char **str, t_here_doc **here_doc)
{
	(*here_doc)->limiter = str[1];
}

static void	aux_here_doc_p(t_here_doc **here_doc)
{
	wait(NULL);
	close ((*here_doc)->fd[WRITE_END]);
	read((*here_doc)->fd[READ_END], /*outfile*/, 1000);
}

void	handle_here_doc(char **str, t_here_doc **here_doc, char **env_var)
{
	if (pipe((*here_doc)->fd) < 0)
			; //call function for do the cleanup
	(*here_doc)->pid = fork();
	if ((*here_doc)->pid == 0)
	{
		char	*line;
		close ((*here_doc)->fd[READ_END]);
		while (1)
		{
			write(1, "pipe here_doc>", 14);
			line = get_next_line(0);
			if (!line || !ft_strncmp(line, (*here_doc)->limiter, ft_strlen(line)))
				; // call funciton for handle it
			write((*here_doc)->fd[WRITE_END], line, ft_strlen(line));
			free(line);
		}
		close ((*here_doc)->fd[WRITE_END]);
	}
}
