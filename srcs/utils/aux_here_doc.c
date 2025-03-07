/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_here_doc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 18:16:15 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/14 21:21:28 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"

static void	aux_check(char *cmd)
{
	ft_printf("pipex: command not found: %s\n", cmd);
}

void	cleanup_here_doc(t_here_doc **here_doc, t_cmd **cmd)
{
	if (here_doc)
	{
		if ((*here_doc)->line)
			free ((*here_doc)->line);
		if ((*here_doc)->save)
		{
			unlink((*here_doc)->save);
			free ((*here_doc)->save);
		}
		if ((*here_doc)->fd[READ_END] != -1)
			close((*here_doc)->fd[READ_END]);
		if ((*here_doc)->fd[WRITE_END] != -1)
			close((*here_doc)->fd[WRITE_END]);
		if ((*here_doc)->outfile_fd != -1)
			close((*here_doc)->outfile_fd);
		if ((*here_doc)->save_fd != -1)
			close ((*here_doc)->save_fd);
	}
	if (cmd && *cmd)
		clean_cmd(cmd);
	free (*here_doc);
	exit (1);
}

void	handle_errors_here_doc(t_here_doc **here_doc, t_cmd **cmd, int index)
{
	if (index == MALLOC_ERROR)
		ft_printf("pipex: malloc error!\n");
	else if (index == PATH_ERROR)
		ft_printf("pipex: couldn't find the path var in env_var!\n");
	else if (index == CMD_NOT_FOUND)
		ft_printf("pipex: command not found: %s\n", (*cmd)->commands[0]);
	else if (index == OPEN_FILE_ERROR)
		ft_printf("pipex: could't open the file!\n");
	else if (index == PROCESS_ERROR)
		ft_printf("pipex: error doing fork!\n");
	else if (index == DUP2_ERROR)
		ft_printf("pipex: error when dup2 is called!\n");
	else if (index == EXEC_ERROR)
		ft_printf("pipex: execve was not executed!\n");
	else if (index == PIPE_CREATE_ERROR)
		ft_printf("pipex: pipe creating error!\n");
	if ((here_doc && *here_doc) || (cmd && *cmd))
		cleanup_here_doc(here_doc, cmd);
	exit (1);
}

void	aux_child(t_here_doc **here_doc, t_cmd **cmd, char **env_var)
{
	restore_save(here_doc);
	if (dup2((*here_doc)->save_fd, STDIN_FILENO) == -1)
		handle_errors_here_doc(here_doc, cmd, DUP2_ERROR);
	if (dup2((*here_doc)->fd[WRITE_END], STDOUT_FILENO) == -1)
		handle_errors_here_doc(here_doc, cmd, DUP2_ERROR);
	execve((*cmd)->path, (*cmd)->commands, env_var);
	handle_errors_here_doc(here_doc, cmd, EXEC_ERROR);
}

int	check_cmds(char **str, char **env_var)
{
	int		i;
	int		j;
	t_cmd	*cmd;
	char	**direc;

	i = 0;
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	direc = ft_split(str[3], ' ');
	if (!cmd || !direc)
		return (1);
	j = find_bin(&cmd, direc[0], env_var);
	matrix_cleanup(direc);
	if (j == -2)
		return (free(cmd), aux_check(str[3]), j);
	free (cmd->path);
	direc = ft_split(str[4], ' ');
	if (!direc)
		return (1);
	i = 0;
	j = find_bin(&cmd, direc[0], env_var);
	matrix_cleanup(direc);
	if (j == -2)
		return (free(cmd), aux_check(str[4]), j);
	free (cmd->path);
	return (free(cmd), 0);
}
