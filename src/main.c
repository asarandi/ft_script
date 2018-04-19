/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 17:05:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/19 07:54:28 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"
#include <time.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>


//int	main(int argc, char **argv, char **envp)

typedef struct		s_stamp
{
	uint64_t		len;
	uint64_t		sec;
	uint32_t		usec;
	uint32_t		direction;
}					t_stamp;

typedef struct	s_script
{
	int				opt_q;
	int				exit_code;
	struct termios	term_cooked;
	struct termios	term_raw;
	struct timespec	ts;
}					t_script;

void		replay_save_time(t_script *sc, t_stamp *st)
{
	sc->ts.tv_sec = st->sec;
	sc->ts.tv_nsec = st->usec * 1000;
}

void		replay_start(t_script *sc, unsigned char *data, int *i)
{
	t_stamp	*st;
	int		size;
	time_t	tclock;

	st = (t_stamp *)&data[(*i)];
	replay_save_time(sc, st);
	size = sizeof(t_stamp);
	size += st->len;
	tclock = st->sec;
	if (sc->opt_q != 1)
		printf("Script started on %s", ctime(&tclock));
	(*i) += size;
	return ;
}

void		replay_end(t_script *sc, unsigned char *data, int *i)
{
	t_stamp	*st;
	int		size;
	time_t	tclock;

	st = (t_stamp *)&data[(*i)];
	size = sizeof(t_stamp);
	size += st->len;
	tclock = st->sec;
	if (sc->opt_q != 1)
		printf("\nScript done on %s", ctime(&tclock));
	(*i) += size;
	return ;

}

void		replay_input(t_script *sc, unsigned char *data, int *i)
{
	t_stamp	*st;
	int		size;

	st = (t_stamp *)&data[(*i)];
	size = sizeof(t_stamp);
	size += st->len;
	(*i) += size;
	return ;
}

void		replay_output(t_script *sc, unsigned char *data, int *i)
{
	t_stamp			*st;
	int				size;
	struct timespec	nap;

	st = (t_stamp *)&data[(*i)];
	size = sizeof(t_stamp);
	nap.tv_sec = st->sec - sc->ts.tv_sec;
	nap.tv_nsec = (st->usec * 1000) - sc->ts.tv_nsec;
	replay_save_time(sc, st);
	if (nap.tv_nsec < 0)
	{
		nap.tv_sec -= 1;
		nap.tv_nsec += 1000000000;
	}
	nanosleep(&nap, NULL);
	write(1, &data[(*i) + size], st->len);
	size += st->len;
	(*i) += size;
	return ;
}

void		replay_invalid(t_script *sc)
{
	sc->exit_code = EXIT_FAILURE;
	ft_fprintf(1, "invalid direction\n");
	return ;
}

void	replay(t_script *sc, unsigned char *data, int filesize)
{
	int		i;
	t_stamp	*st;

	i = 0;
	while (i < filesize)
	{
		st = (t_stamp *)&data[i];
		if (st->direction == 's')
			replay_start(sc, data, &i);
		else if (st->direction == 'e')
			replay_end(sc, data, &i);
		else if (st->direction == 'i')
			replay_input(sc, data, &i);
		else if (st->direction == 'o')
			replay_output(sc, data, &i);
		else
			return (replay_invalid(sc));

	}
}

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
	if (ioctl(STDIN_FILENO, TIOCGETA, &sc->term_cooked) == -1)
		return exit_failure_unmap_msg(sc, "ioctl() failed.");
	sc->term_raw = sc->term_cooked;
	termios_makeraw(&sc->term_raw);
	sc->term_raw.c_lflag &= ~ECHO;


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
