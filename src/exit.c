/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:34:39 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:34:52 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	exit_failure_msg(char *msg)
{
	ft_fprintf(1, "%s\n", msg);
	return (EXIT_FAILURE);
}

int	exit_failure_unmap_msg(t_script *sc, char *msg)
{
	munmap(sc, sizeof(t_script));
	return (exit_failure_msg(msg));
}
