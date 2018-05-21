/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:21 by asarandi          #+#    #+#             */
/*   Updated: 2018/05/21 03:59:26 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

char	*find_shell(t_script *sc)
{
	int	i;

	if (sc->envp == NULL)
		return (NULL);
	i = 0;
	while (sc->envp[i] != NULL)
	{
		if (ft_strncmp(sc->envp[i], "SHELL=", 6) == 0)
			return (&sc->envp[i][6]);
		i++;
	}
	return (NULL);
}

int		record_shell(t_script *sc)
{
	char	*argv[2];

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
	argv[1] = NULL;
	argv[0] = find_shell(sc);
	if ((argv[0] == NULL) || (argv[0][0] == 0))
		execve("/bin/sh", NULL, sc->envp);
	else
		execve(argv[0], argv, sc->envp);
	ft_fprintf(1, "execve() failed.\n");
	(void)kill(0, SIGTERM);
	return (record_done(sc, EXIT_FAILURE));
}
