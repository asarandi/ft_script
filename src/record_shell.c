/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:21 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:44:52 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int record_shell(t_script *sc)
{
	(void)close(sc->master);
	(void)close(sc->fd);
	(void)setsid();
	if (ioctl(sc->slave, TIOCSCTTY, (char *)NULL) == -1)
		return (-1);
	(void)dup2(sc->slave, 0);
	(void)dup2(sc->slave, 1);
	(void)dup2(sc->slave, 2);
	if (sc->slave > 2)
		(void)close(sc->slave);
	execve("/bin/sh", NULL, sc->envp);
	ft_fprintf(1, "execve() failed.\n");
	(void)kill(0, SIGTERM);
	return (record_done(sc, EXIT_FAILURE));
}
