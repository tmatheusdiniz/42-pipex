/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_clean.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mreinald <mreinald@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:39:53 by mreinald          #+#    #+#             */
/*   Updated: 2025/02/11 20:40:51 by mreinald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/pipex.h"

void	matrix_cleanup(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		free (matrix[i++]);
	free (matrix);
}
