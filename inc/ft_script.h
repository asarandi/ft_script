/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 17:07:05 by asarandi          #+#    #+#             */
/*   Updated: 2018/05/21 03:22:16 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCRIPT_H
# define FT_SCRIPT_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <sys/time.h>
# include <termios.h>
# include <fcntl.h>
# include <time.h>
# include <signal.h>
# include "libft.h"
# include "libftprintf.h"
# include "functions.h"
# define BUFSIZ 0x10000
# define DEFAULTFILENAME "typescript"
# define FILE_TRUNCATE	(O_WRONLY | O_CREAT | O_TRUNC)
# define FILE_APPEND	(O_WRONLY | O_CREAT | O_APPEND)

typedef struct		s_stamp
{
	uint64_t		len;
	uint64_t		sec;
	uint32_t		usec;
	uint32_t		direction;
}					t_stamp;

typedef struct		s_script
{
	int				option_q;
	int				option_a;
	int				action;
	int				exit_code;
	char			*filename;
	int				fd;
	int				master;
	int				slave;
	struct termios	term_cooked;
	struct termios	term_raw;
	struct timespec	ts;
	int				child;
	int				argc;
	char			**argv;
	char			**envp;
}					t_script;

#endif
