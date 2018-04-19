/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 11:40:50 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/19 16:32:35 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "ft_script.h"

typedef struct s_script	t_script;
typedef struct s_stamp	t_stamp;

char	*file_get_contents(char *filename);
int		file_get_size(char *filename);
int		create_pty(t_script *sc);
int		create_pty_failed_close_fd(int fd);
int		create_pty_set_slave_settings(t_script *sc);
int		exit_failure_msg(char *msg);
int		exit_failure_unmap_msg(t_script *sc, char *msg);
int		main();
void	*file_mmap(char *filename);
void	replay(t_script *sc, unsigned char *data, int filesize);
void	replay_end(t_script *sc, unsigned char *data, int *i);
void	replay_input(t_script *sc, unsigned char *data, int *i);
void	replay_invalid(t_script *sc);
void	replay_output(t_script *sc, unsigned char *data, int *i);
void	replay_save_time(t_script *sc, t_stamp *st);
void	replay_start(t_script *sc, unsigned char *data, int *i);
void	termios_makeraw(struct termios *t);

#endif
