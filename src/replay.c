/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replay.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 13:36:55 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:31:40 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

void	replay_save_time(t_script *sc, t_stamp *st)
{
	sc->ts.tv_sec = st->sec;
	sc->ts.tv_nsec = st->usec * 1000;
	return ;
}

void	replay_input(t_script *sc, unsigned char *data, int *i)
{
	t_stamp	*st;

	sc->ts.tv_sec += 0;
	st = (t_stamp *)&data[(*i)];
	(*i) += sizeof(t_stamp) + st->len;
	return ;
}

void	replay_output(t_script *sc, unsigned char *data, int *i)
{
	t_stamp			*st;
	struct timespec	nap;

	st = (t_stamp *)&data[(*i)];
	nap.tv_sec = st->sec - sc->ts.tv_sec;
	nap.tv_nsec = (st->usec * 1000) - sc->ts.tv_nsec;
	if (nap.tv_nsec < 0)
	{
		nap.tv_sec -= 1;
		nap.tv_nsec += 1000000000;
	}
	nanosleep(&nap, NULL);
	replay_save_time(sc, st);
	write(1, &data[(*i) + sizeof(t_stamp)], st->len);
	(*i) += sizeof(t_stamp) + st->len;
	return ;
}

void	replay_invalid(t_script *sc)
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
