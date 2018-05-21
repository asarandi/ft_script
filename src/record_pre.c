/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record_pre.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:43:10 by asarandi          #+#    #+#             */
/*   Updated: 2018/05/21 03:42:14 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	record_pre(t_script *sc)
{
	if (sc->option_a == 1)
	{
		if ((sc->fd = open(sc->filename, FILE_APPEND, 0644)) < 0)
			return (exit_failure_unmap_msg(sc, "open() failed"));
	}
	else
	{
		if ((sc->fd = open(sc->filename, FILE_TRUNCATE, 0644)) < 0)
			return (exit_failure_unmap_msg(sc, "open() failed"));
	}
	if (ioctl(STDIN_FILENO, TIOCGETA, &sc->term_cooked) == -1)
		return (exit_failure_unmap_msg(sc, "ioctl() failed."));
	if (create_pty(sc) == -1)
		return (exit_failure_unmap_msg(sc, "create_pty() failed"));
	sc->term_raw = sc->term_cooked;
	termios_makeraw(&sc->term_raw);
	sc->term_raw.c_lflag &= ~ECHO;
	if (sc->option_q == 0)
		ft_fprintf(1, "Script started, output file is %s\n", sc->filename);
	if (ioctl(STDIN_FILENO, TIOCSETAF, &sc->term_raw) == -1)
		return (exit_failure_unmap_msg(sc, "ioctl() failed."));
	return (0);
}
