/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replay_start_end.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:31:19 by asarandi          #+#    #+#             */
/*   Updated: 2018/05/21 03:23:19 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

void	replay_start(t_script *sc, unsigned char *data, int *i)
{
	t_stamp	*st;
	time_t	tclock;

	st = (t_stamp *)&data[(*i)];
	replay_save_time(sc, st);
	tclock = st->sec;
	ft_fprintf(1, "Script started on %s", ctime(&tclock));
	(*i) += sizeof(t_stamp) + st->len;
	return ;
}

void	replay_end(t_script *sc, unsigned char *data, int *i)
{
	t_stamp	*st;
	time_t	tclock;

	(void)sc;
	st = (t_stamp *)&data[(*i)];
	tclock = st->sec;
	ft_fprintf(1, "\nScript done on %s", ctime(&tclock));
	(*i) += sizeof(t_stamp) + st->len;
	return ;
}
