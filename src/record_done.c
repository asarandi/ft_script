/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record_done.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:42:41 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:43:01 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	record_done(t_script *sc, int exit_code)
{
	struct timeval	tv;

	(void)ioctl(STDIN_FILENO, TIOCSETAF, &sc->term_cooked);
	(void)gettimeofday(&tv, NULL);
	(void)ft_fprintf(1, "\nScript done on %s", exit_code, ctime(&tv.tv_sec));
	(void)record_write(sc, "", 0, 'e');
	(void)close(sc->fd);
	(void)close(sc->master);
	(void)munmap(sc, sizeof(t_script));
	_exit(exit_code);
}
