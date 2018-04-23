/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:45:37 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:45:55 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int record_wait(t_script *sc)
{
	int	exit_code;
	int	status;

	exit_code = 1;
	if (waitpid(sc->child, &status, 0) == sc->child)
	{
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = WTERMSIG(status);
		else
			exit_code = 1;
	}
	return (record_done(sc, exit_code));
}
