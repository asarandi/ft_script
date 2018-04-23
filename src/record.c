/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:43:47 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:44:10 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	record(t_script *sc)
{
	fd_set			rfd;
	int				readstdin;
	struct timeval	tv;
	int				i;
	int				n;

	(void)record_write(sc, "", 0, 's');
	readstdin = 1;
	i = 30;
	while (1)
	{
		FD_ZERO(&rfd);
		FD_SET(sc->master, &rfd);
		if (readstdin)
			FD_SET(STDIN_FILENO, &rfd);
		tv.tv_sec = 1;						/* XXX */
		tv.tv_usec = 0;
		if ((n = select(sc->master + 1, &rfd, 0, 0, &tv)) < 0)
			return (exit_failure_unmap_msg(sc, "select() failed."));
		if ((n > 0) && (FD_ISSET(STDIN_FILENO, &rfd)))
		{
			if (record_input(sc, &readstdin) == -1)
				break ;
		}
		if ((n > 0) && (FD_ISSET(sc->master, &rfd)))
		{
			if (record_output(sc) == -1)
				break ;
		}
	}
	record_wait(sc);
	return (record_done(sc, 42));
}
