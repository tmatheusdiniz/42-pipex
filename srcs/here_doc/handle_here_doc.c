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
#include <stdlib.h>
#include <unistd.h>

static void	aux_here_doc_c(t_here_doc **here_doc, t_cmd **cmd, char **env_var);
static void	aux_here_doc_p(t_here_doc **here_doc,
				t_cmd **cmd, char **str, char **env_var);
static void	aux_child_two(t_here_doc **here_doc,
				t_cmd **cmd, char **env_var);

static int	init_here_doc(t_here_doc **here_doc, t_cmd **cmd, char **str)
{
	*here_doc = (t_here_doc *)malloc(sizeof(t_here_doc));
	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!*here_doc || !*cmd)
		return (1);
	(*here_doc)->line = NULL;
	(*cmd)->path = NULL;
	(*cmd)->commands = NULL;
	(*cmd)->pid = NULL;
	(*here_doc)->fd[READ_END] = -1;
	(*here_doc)->fd[WRITE_END] = -1;
	(*here_doc)->outfile_fd = -1;
	(*here_doc)->save_fd = -1;
	(*here_doc)->limiter = str[2];
	(*here_doc)->outfile = str[5];
	if (!(*here_doc)->limiter)
		return (close ((*here_doc)->save_fd),
			close((*here_doc)->outfile_fd), free(*here_doc), 0);
	(*here_doc)->save = ft_strdup("here_doc_tmp");
	if (!(*here_doc)->save)
		return (free(*here_doc), 1);
	return (0);
}

static void	aux_child_two(t_here_doc **here_doc, t_cmd **cmd, char **env_var)
{
	close ((*here_doc)->fd[WRITE_END]);
	if (dup2((*here_doc)->fd[READ_END], STDIN_FILENO) == -1)
		handle_errors_here_doc (here_doc, cmd, 5);
	if (dup2((*here_doc)->outfile_fd, STDOUT_FILENO) == -1)
		handle_errors_here_doc (here_doc, cmd, 5);
	execve((*cmd)->path, (*cmd)->commands, env_var);
	handle_errors_here_doc(here_doc, cmd, EXEC_ERROR);
}

static void	aux_here_doc_c(t_here_doc **here_doc, t_cmd **cmd, char **env_var)
{
	size_t	size_limiter;

	close ((*here_doc)->fd[READ_END]);
	size_limiter = ft_strlen((*here_doc)->limiter);
	while (1)
	{
		write(1, "pipe here_doc>", 14);
		(*here_doc)->line = get_next_line(0);
		if (!(*here_doc)->line)
			handle_errors_here_doc(here_doc, cmd, 1);
		if (!ft_strncmp((*here_doc)->line, (*here_doc)->limiter, size_limiter)
			&& ((*here_doc)->line[size_limiter] == '\n'
				|| (*here_doc)->line[size_limiter] == '\0'))
		{
			free ((*here_doc)->line);
			break ;
		}
		write((*here_doc)->save_fd, (*here_doc)->line,
			ft_strlen((*here_doc)->line));
		free((*here_doc)->line);
	}
	aux_child(here_doc, cmd, env_var);
}

static void	aux_here_doc_p(t_here_doc **here_doc,
		t_cmd **cmd, char **str, char **env_var)
{
	int	i;

	waitpid((*here_doc)->pid, NULL, 0);
	close((*here_doc)->fd[WRITE_END]);
	(*here_doc)->pid = fork();
	if ((*here_doc)->pid < 0)
		handle_errors_here_doc(here_doc, cmd, PROCESS_ERROR);
	if ((*here_doc)->pid == 0)
	{
		(*cmd)->commands = ft_split(str[4], ' ');
		if (!(*cmd)->commands)
			handle_errors_here_doc(here_doc, cmd, MALLOC_ERROR);
		i = find_bin(cmd, (*cmd)->commands[0], env_var);
		if (i == MALLOC_ERROR || i == PATH_ERROR || i == CMD_NOT_FOUND)
			handle_errors_here_doc(here_doc, cmd, i);
		aux_child_two(here_doc, cmd, env_var);
	}
	else
	{
		waitpid((*here_doc)->pid, NULL, 0);
		close ((*here_doc)->fd[WRITE_END]);
		close ((*here_doc)->fd[READ_END]);
		cleanup_here_doc(here_doc, cmd);
	}
}

void	handle_here_doc(t_here_doc **here_doc,
		t_cmd **cmd, char **str, char **env_var)
{
	int	i;

	if (init_here_doc(here_doc, cmd, str))
		handle_errors_here_doc(here_doc, cmd, MALLOC_ERROR);
	if (open_outfile_here(here_doc) == 4)
		handle_errors_here_doc(here_doc, cmd, 4);
	if (pipe((*here_doc)->fd) < 0)
		handle_errors_here_doc(here_doc, cmd, 7);
	(*here_doc)->pid = fork();
	if ((*here_doc)->pid == 0)
	{
		(*cmd)->commands = ft_split(str[3], ' ');
		if (!(*cmd)->commands)
			handle_errors_here_doc(here_doc, cmd, 1);
		i = find_bin(cmd, (*cmd)->commands[0], env_var);
		if (i == 1 || i == 2)
			handle_errors_here_doc(here_doc, cmd, i);
		else if (i == -2)
			handle_errors_here_doc(here_doc, cmd, -2);
		aux_here_doc_c(here_doc, cmd, env_var);
	}
	else
		aux_here_doc_p(here_doc, cmd, str, env_var);
}
