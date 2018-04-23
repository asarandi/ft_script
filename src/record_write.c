/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:45:02 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:45:25 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

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
