/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 17:05:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/19 16:34:20 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"


//int	main(int argc, char **argv, char **envp)
//void	replay(t_script *sc, unsigned char *data, int filesize);

void	termios_makeraw(struct termios *t)
{
	t->c_iflag &= ~(IMAXBEL|IXOFF|INPCK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON|IGNPAR);
	t->c_iflag |= IGNBRK;
	t->c_oflag &= ~OPOST;
	t->c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHONL|ICANON|ISIG|IEXTEN|NOFLSH|TOSTOP|PENDIN);
	t->c_cflag &= ~(CSIZE|PARENB);
	t->c_cflag |= CS8|CREAD;
	t->c_cc[VMIN] = 1;
	t->c_cc[VTIME] = 0;
}

int	exit_failure_msg(char *msg)
{
	ft_fprintf(1, "%s\n", msg);
	return (EXIT_FAILURE);
}

int	exit_failure_unmap_msg(t_script *sc, char *msg)
{
	munmap(sc, sizeof(t_script));
	return (exit_failure_msg(msg));
}

int	main()
{
	unsigned char	*data;
	int				filesize;
	char			*filename = "typescript";
	int				r;
	t_script		*sc;


	sc = mmap(NULL, sizeof(t_script), PROT_NONE | PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
	if (sc == MAP_FAILED)
		return exit_failure_msg("mmap() failed.");
	if (create_pty(sc) == -1)
		return exit_failure_msg("create_pty() failed.");



/*	if (ioctl(STDIN_FILENO, TIOCGETA, &sc->term_cooked) == -1)
		return exit_failure_unmap_msg(sc, "ioctl() failed.");
	sc->term_raw = sc->term_cooked;
	termios_makeraw(&sc->term_raw);
	sc->term_raw.c_lflag &= ~ECHO;
*/

//	ft_printf("ft_script\n");
	filesize = file_get_size(filename);
//	ft_fprintf(1, "type script file size = %d\n", filesize);
	data = file_mmap(filename);


	if (data != NULL)
	{
//		if (ioctl(STDIN_FILENO, TIOCSETAF, &sc->term_raw) == -1)
//			return exit_failure_unmap_msg(sc, "ioctl() failed.");
		replay(sc, data, filesize);
//		if (ioctl(STDIN_FILENO, TIOCSETAF, &sc->term_cooked) == -1)
//			return exit_failure_unmap_msg(sc, "ioctl() failed.");


//		ft_fprintf(1, "mapped typescript file to %p\n", data);
		r = munmap(data, filesize);
//		ft_fprintf(1, "munmap returns %d\n", r);
	}

	r = sc->exit_code;
	munmap(sc, sizeof(t_script));

	return (r);
}
