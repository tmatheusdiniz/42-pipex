/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:37:39 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/10 14:58:21 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/Libft/include/libft.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Error defines
#define MALLOC_ERROR		1
#define PATH_ERROR			2
#define CMD_NOT_FOUND		-2
#define PROCESS_ERROR		3
#define OPEN_FILE_ERROR		4
#define DUP2_ERROR			5
#define EXEC_ERROR			6
#define PIPE_CREATE_ERROR	7

// Read and Write ends
#define READ_END 0
#define WRITE_END 1

typedef struct s_cmd
{
	char	*path;
	char	*infile;
	char	*outfile;
	char	**commands;
	pid_t	*pid;
}	t_cmd;

typedef struct s_pipex
{
	int		**pipe;
	int		fd_in;
	int		fd_out;
	int		number_commands;
}	t_pipex;

typedef struct s_here_doc
{
	pid_t	pid;
	int		fd[2];
	int		outfile_fd;
	int		save_fd;
	char	*limiter;
	char	*line;
	char	*outfile;
	char	*save;
}	t_here_doc;

// Core
int		main(int v, char **str, char **env_var);

// Handlers
void	handle_forks(char **str, t_cmd **cmd, t_pipex **pipex, char **env_var);
void	handle_errors(t_cmd **cmd, t_pipex **pipex, int index);

// Parse
int		find_bin(t_cmd **cmd, char *command, char **env_var);
int		open_files(t_pipex **pipex, char *infile, char *outfile);

// Here_doc
void	handle_here_doc(t_here_doc **here_doc,
			t_cmd **cmd, char **str, char **env_var);
int		open_outfile_here(t_here_doc **here_doc);
void	handle_errors_here_doc(t_here_doc **here_doc, t_cmd **cmd, int index);

// Cleanup
void	clean_cmd(t_cmd **cmd);
void	clean_pipex(t_pipex **pipex);
void	cleanup_here_doc(t_here_doc **here_doc, t_cmd **cmd);
void	matrix_cleanup(char **matrix);
