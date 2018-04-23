/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 17:05:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/23 02:44:51 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	main_replay(t_script *sc)
{
	unsigned char	*data;
	int				filesize;

	filesize = file_get_size(sc->filename);
	data = file_mmap(sc->filename);
	if (data != NULL)
		replay(sc, data, filesize);
	return (sc->exit_code);
}

int	main_record(t_script *sc)
{
	int		n;

	if ((n = record_pre(sc)) != 0)
		return (n);
	if ((sc->child = fork()) < 0)
	{
		ft_fprintf(1, "fork() failed.\n");
		return (record_done(sc, 1));
	}
	if (sc->child == 0)
		return (record_shell(sc));
	close(sc->slave);
	return (record(sc));
}

int	main(int argc, char **argv, char **envp)
{
	t_script		*sc;

	if (argc > 1)
	{
		sc = mmap(NULL, sizeof(t_script), PROT_NONE | PROT_READ |
				PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
		if (sc == MAP_FAILED)
			return (exit_failure_msg("mmap() failed."));
		sc->argc = argc;
		sc->argv = argv;
		sc->envp = envp;
		sc->filename = DEFAULTFILENAME;
		if (ft_strcmp(argv[1], "-r") == 0)
			return (main_record(sc));
		if (ft_strcmp(argv[1], "-p") == 0)
			return (main_replay(sc));
		munmap(sc, sizeof(t_script));
	}
	return (0);
}
