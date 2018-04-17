/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 05:24:10 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/17 11:38:12 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

char	*file_get_contents(char *filename)
{
	int			fd;
	struct stat	st;
	char		*data;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (NULL);
	data = NULL;
	if ((fstat(fd, &st)) == 0)
	{
		if ((data = ft_memalloc(st.st_size + 1)) == NULL)
		{
			(void)close(fd);
			return (NULL);
		}
		if ((read(fd, data, st.st_size)) != st.st_size)
		{
			free(data);
			data = NULL;
		}
	}
	(void)close(fd);
	return (data);
}

int		file_get_size(char *filename)
{
	int			filesize;
	int			fd;
	struct stat	st;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (-1);
	filesize = -1;
	if ((fstat(fd, &st)) == 0)
	{
		filesize = st.st_size;
	}
	(void)close(fd);
	return (filesize);
}

void	*file_mmap(char *filename)
{
	int			fd;
	struct stat	st;
	void		*data;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (NULL);
	data = NULL;
	if ((fstat(fd, &st)) == 0)
	{
		data = mmap(NULL, st.st_size, PROT_NONE | PROT_READ, MAP_PRIVATE, fd, 0);
		if (data == MAP_FAILED)
			data = NULL;
	}
	close(fd);
	return (data);
}
