/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 18:02:47 by eniini            #+#    #+#             */
/*   Updated: 2021/01/12 23:11:04 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Simple data saving functionality that allows the user to save and load
**	a specific fdf state between individual processes.
**	Necessary open() calls are handled by deal_key_savestate().
*/

#include "fdf.h"

/*
**	Saves relevant variables to the file descriptor.
**	Linebreak is used as delimiter for loading back the data.
*/

static void	savestate_2(t_fdf *fdf, int fd)
{
	ft_putnbr_fd(fdf->col.high, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->col.mid, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->col.low, fd);
	ft_putchar_fd('\n', fd);
}

void		save_state(t_fdf *fdf, int fd)
{
	ft_putnbr_fd(fdf->map.proj, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->map.z_val, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->map.zoom, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->map.o_y, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->map.o_x, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->map.x_r, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->map.y_r, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->map.z_r, fd);
	ft_putchar_fd('\n', fd);
	ft_putnbr_fd(fdf->col.shaded, fd);
	ft_putchar_fd('\n', fd);
	savestate_2(fdf, fd);
	if (close(fd))
		ft_getout(strerror(errno));
	ft_putendl("fdf state saved to file!");
}

/*
**	Reads stored values of [fdf->map] from the given file descriptor.
*/

void		load_state(t_fdf *fdf, int fd)
{
	char		*buffer[12];
	char		*line;
	int			i;

	i = 0;
	while (get_next_line(fd, &line))
		buffer[i++] = line;
	buffer[i] = NULL;
	fdf->map.proj = ft_atoi(buffer[0]);
	fdf->map.z_val = ft_atoi(buffer[1]);
	fdf->map.zoom = ft_atoi(buffer[2]);
	fdf->map.o_y = ft_atoi(buffer[3]);
	fdf->map.o_x = ft_atoi(buffer[4]);
	fdf->map.x_r = ft_atoi(buffer[5]);
	fdf->map.y_r = ft_atoi(buffer[6]);
	fdf->map.z_r = ft_atoi(buffer[7]);
	fdf->col.shaded = ft_atoi(buffer[8]);
	fdf->col.high = ft_atoi(buffer[9]);
	fdf->col.mid = ft_atoi(buffer[10]);
	fdf->col.low = ft_atoi(buffer[11]);
	i = 0;
	while (buffer[i] != NULL)
		free(buffer[i++]);
	if (close(fd))
		ft_getout(strerror(errno));
}
