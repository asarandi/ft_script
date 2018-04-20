/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 17:05:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/20 04:24:15 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"


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

int	record_write(t_script *sc, char *buf, int size, int direction)
{
	t_stamp			stamp;
	struct timeval	tv;

	(void)gettimeofday(&tv, NULL);
	stamp.len = size;
	stamp.sec = tv.tv_sec;
	stamp.usec = tv.tv_usec;
	stamp.direction = direction;
	if (write(sc->fd, &stamp, sizeof(stamp)) == -1)
		return (-1);
	if (write(sc->fd, buf, size) == -1)
		return (-1);
	return (0);
}

int	record_pre(t_script *sc)
{
	if ((sc->fd = open(sc->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
		return exit_failure_unmap_msg(sc, "open() failed");
	if (ioctl(STDIN_FILENO, TIOCGETA, &sc->term_cooked) == -1)
		return exit_failure_unmap_msg(sc, "ioctl() failed.");
	if (create_pty(sc) == -1)
		return (exit_failure_unmap_msg(sc, "create_pty() failed"));
	sc->term_raw = sc->term_cooked;
	termios_makeraw(&sc->term_raw);
	sc->term_raw.c_lflag &= ~ECHO;
	if (ioctl(STDIN_FILENO, TIOCSETAF, &sc->term_raw) == -1)
		return exit_failure_unmap_msg(sc, "ioctl() failed.");
	return (0);
}


int	record_input(t_script *sc, int *readstdin)
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
		*readstdin = 0;
	}
	if (cc > 0)
	{
		// if (rawout) record_write
		record_write(sc, ibuf, cc, 'i');
		(void)write(sc->master, ibuf, cc);
		// if kflg .. something something darkside
	}
	return (0);
}

int	record_output(t_script *sc)
{
	int				cc;
	char			obuf[BUFSIZ];

	cc = read(sc->master, obuf, BUFSIZ);
	if (cc <= 0)
		return (-1);
	(void)write(STDOUT_FILENO, obuf, cc);
	record_write(sc, obuf, cc, 'o');
	return (0);
}

int	record_done(t_script *sc, int exit_code)
{
	struct timeval	tv;

	(void)ioctl(STDIN_FILENO, TIOCSETAF, &sc->term_cooked);
	(void)gettimeofday(&tv, NULL);
	(void)ft_fprintf(1, "\nScript done on %s", exit_code, ctime(&tv.tv_sec));
	(void)record_write(sc, "", 0, 'e');
	(void)close(sc->fd);
	(void)close(sc->master);
	(void)munmap(sc, sizeof(t_script));
	_exit(exit_code);
}

int record_wait(t_script *sc)
{
	int	exit_code;
	int	status;

	exit_code = 1;
	if (waitpid(sc->child, &status, 0) == sc->child)
	{
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = WTERMSIG(status);
		else
			exit_code = 1;
	}
	return (record_done(sc, exit_code));
}

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
