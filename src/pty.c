/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 15:15:06 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/19 16:28:19 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	create_pty_failed_close_fd(int fd)
{
	(void)close(fd);
	return (-1);
}

int	create_pty_set_slave_settings(t_script *sc)
{
	struct termios	term;
	struct winsize	winsz;

	if (ioctl(STDIN_FILENO, TIOCGETA, &term) != -1)
		(void)ioctl(sc->slave, TIOCSETAF, &term);
	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &winsz) != -1)
		(void)ioctl(sc->slave, TIOCSWINSZ, &winsz);
	return (0);
}

int	create_pty(t_script *sc)
{
	int			ms;
	int			sl;
	struct stat	st;
	static char	buf[128];

	if ((ms = open("/dev/ptmx", O_RDWR | O_NOCTTY)) < 0)
		return (-1);
	if ((ioctl(ms, TIOCPTYGRANT) < 0) || (ioctl(ms, TIOCPTYUNLK) < 0))
		return (create_pty_failed_close_fd(ms));
	if ((ioctl(ms, TIOCPTYGNAME, &buf) != 0) || (stat(&buf[0], &st) != 0))
		return (create_pty_failed_close_fd(ms));
	if ((sl = open(&buf[0], O_RDWR | O_NOCTTY)) < 0)
		return (create_pty_failed_close_fd(ms));
	sc->master = ms;
	sc->slave = sl;
	(void)create_pty_set_slave_settings(sc);
	return (0);
}
