/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:48:53 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/10 20:22:43 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"
#include <unistd.h>

static char	*find_path(char **env_var);

static void	clean_split(char **split, int index)
{
	int	i;

	i = 0;
	while (i < index)
		free(split[i++]);
	free (split);
}

static char	*find_path(char **env_var)
{
	while (*env_var)
	{
		if (!ft_strncmp(*env_var, "PATH=", 5))
			return (*env_var + 5);
		env_var ++;
	}
	return (NULL);
}

int	find_bin(t_cmd **cmd, char *command, char **env_var)
{
	int		i;
	char	*path;
	char	*temp_path;
	char	**directories;

	i = 0;
	path = find_path(env_var);
	if (!path)
		return (2);
	directories = ft_split(path, ':');
	if (!directories)
		return (1);
	while (directories[i])
	{
		temp_path = ft_strjoin(directories[i], "/");
		path = ft_strjoin(temp_path, command);
		free (temp_path);
		if (access(path, X_OK) == 0)
			return (clean_split(directories, i), (*cmd)->path = path, 0);
		free (path);
		++ i;
	}
	return (clean_split(directories, i), -2);
}
