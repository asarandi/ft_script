/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:46:37 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/23 03:25:55 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	record_input(t_script *sc)
{
	int				cc;
	char			ibuf[BUFSIZ];
	struct termios	stt;

	cc = read(STDIN_FILENO, ibuf, BUFSIZ);
	if (cc < 0)
		return (-1);
	if (cc == 0)
	{
		if ((ioctl(sc->master, TIOCGETA, &stt) == 0) &&
				((stt.c_lflag & ICANON) != 0))
			(void)write(sc->master, &stt.c_cc[VEOF], 1);
	}
	if (cc > 0)
	{
		record_write(sc, ibuf, cc, 'i');
		(void)write(sc->master, ibuf, cc);
	}
	return (0);
}
