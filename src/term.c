/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:35:24 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/23 02:01:46 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

void	termios_makeraw(struct termios *t)
{
	t->c_iflag &= ~(IMAXBEL | IXOFF | INPCK | BRKINT | PARMRK |
			ISTRIP | INLCR | IGNCR | ICRNL | IXON | IGNPAR);
	t->c_iflag |= IGNBRK;
	t->c_oflag &= ~OPOST;
	t->c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON |
			ISIG | IEXTEN | NOFLSH | TOSTOP | PENDIN);
	t->c_cflag &= ~(CSIZE | PARENB);
	t->c_cflag |= CS8 | CREAD;
	t->c_cc[VMIN] = 1;
	t->c_cc[VTIME] = 0;
}
