/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 17:05:27 by asarandi          #+#    #+#             */
/*   Updated: 2018/04/17 11:45:53 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

//int	main(int argc, char **argv, char **envp)
int	main()
{
	unsigned char	*data;
	int				filesize;
	char			*filename = "typescript";
	int				r;

	ft_printf("ft_script\n");
	filesize = file_get_size(filename);
	ft_fprintf(1, "type script file size = %d\n", filesize);
	data = file_mmap(filename);
	if (data != NULL)
	{
		ft_fprintf(1, "mapped typescript file to %p\n", data);
		r = munmap(data, filesize);
		ft_fprintf(1, "munmap returns %d\n", r);
	}

	return (0);
}
