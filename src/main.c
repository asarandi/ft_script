/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 17:05:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/05/21 03:56:22 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int	main_record(t_script *sc)
{
	int	n;

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

int	main_replay(t_script *sc)
{
	unsigned char	*data;
	int				filesize;

	filesize = file_get_size(sc->filename);
	data = file_mmap(sc->filename);
	if (data != NULL)
	{
		replay(sc, data, filesize);
		(void)munmap(data, filesize);
	}
	else
		ft_fprintf(1, "%s: %s: Failed to open file\n",
				sc->argv[0], sc->filename);
	return (sc->exit_code);
}

int	show_usage(t_script *sc)
{
	ft_fprintf(1, "[usage] %s -a -q -r -p <filename>\n", sc->argv[0]);
	ft_fprintf(1, "default file name is \"typescript\"\n");
	ft_fprintf(1, "\toptions:\n");
	ft_fprintf(1, "\t-a\tappend to typescript file\n");
	ft_fprintf(1, "\t-q\tquiet mode\n");
	ft_fprintf(1, "\t-r\trecord to typescript file (default)\n");
	ft_fprintf(1, "\t-p\tplayblack from typescript file\n");
	sc->action = 0;
	return (0);
}

int	parse_args(t_script *sc)
{
	int	i;

	i = 1;
	while (i < sc->argc)
	{
		if (ft_strcmp(sc->argv[i], "-a") == 0)
			sc->option_a = 1;
		else if (ft_strcmp(sc->argv[i], "-q") == 0)
			sc->option_q = 1;
		else if (ft_strcmp(sc->argv[i], "-r") == 0)
			sc->action = 1;
		else if (ft_strcmp(sc->argv[i], "-p") == 0)
			sc->action = 2;
		else
		{
			if ((i == sc->argc - 1) && (sc->argv[i][0] != '-'))
				sc->filename = sc->argv[i];
			else
				return (show_usage(sc));
		}
		i++;
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_script		*sc;
	int				ret;

	ret = 0;
	sc = mmap(NULL, sizeof(t_script), PROT_NONE |
			PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
	if (sc == MAP_FAILED)
		return (exit_failure_msg("mmap() failed."));
	sc->argc = argc;
	sc->argv = argv;
	sc->envp = envp;
	sc->action = 1;
	sc->filename = DEFAULTFILENAME;
	(void)parse_args(sc);
	if (sc->action == 1)
		sc->exit_code = main_record(sc);
	else if (sc->action == 2)
		sc->exit_code = main_replay(sc);
	ret = sc->exit_code;
	munmap(sc, sizeof(t_script));
	return (ret);
}
