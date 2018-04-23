/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 17:05:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:46:00 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	main(int argc, char **argv, char **envp)
{
	unsigned char	*data;
	int				filesize;
	char			*filename = "typescript";
	int				r;
	t_script		*sc;
	int				n;


	sc = mmap(NULL, sizeof(t_script), PROT_NONE | PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
	if (sc == MAP_FAILED)
		return exit_failure_msg("mmap() failed.");

	sc->argc = argc;
	sc->argv = argv;
	sc->envp = envp;
	sc->filename = filename;

	if ((argc > 1) && (ft_strcmp(argv[1], "-r") == 0))
	{	
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
	if ((argc > 1) && (ft_strcmp(argv[1], "-p") == 0))
	{
		filesize = file_get_size(filename);
		data = file_mmap(filename);
		if (data != NULL)
		{
			replay(sc, data, filesize);
			r = munmap(data, filesize);
		}
		r = sc->exit_code;
		munmap(sc, sizeof(t_script));
		return (r);
	}
	return (0);
}
