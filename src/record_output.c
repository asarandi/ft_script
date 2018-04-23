/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record_output.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:37:51 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/22 17:47:34 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

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
