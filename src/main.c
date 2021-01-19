/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 13:59:06 by eniini            #+#    #+#             */
/*   Updated: 2021/01/19 13:37:06 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Draws a rasterization of an elevation map using the mlx/x11 libraries.
**	In a case of error, either strerror() or a custom message is printed on
**	standard output.
*/

#include "fdf.h"

/*
**	Initializes fdf struct values on program startup.
**	Can be called afterwards by user to reset the visualization.
*/

void	resetmap(t_fdf *fdf)
{
	fdf->map.o_x = 0;
	fdf->map.o_y = 0;
	fdf->map.x_r = 0;
	fdf->map.y_r = 0;
	fdf->map.z_r = 0;
	fdf->map.z_val = 10;
	fdf->map.angle_y = 0.5;
	fdf->map.angle_z = 0.25;
	fdf->map.proj = 1;
	fdf->map.zoom = (fdf->map.width > fdf->map.height) \
	? (WIN_WIDTH / fdf->map.width) \
	: (WIN_HEIGHT / fdf->map.height);
	fdf->col.high = DEF_HIGH;
	fdf->col.mid = DEF_MID;
	fdf->col.low = DEF_LOW;
	fdf->col.shaded = 0;
	fdf->loop = 0;
}

/*
**	Initializes fdf structs and mlx elements,
**	then reads z-values from the map given as argument into a 2d array.
**
**	Also initializes mlx image and stores its relevant data to [fdf->img].
*/

t_fdf	*init(char *argv)
{
	t_fdf	*fdf;

	if (!(fdf = (t_fdf*)malloc(sizeof(t_fdf))))
		ft_getout(strerror(errno));
	read_map(argv, fdf);
	resetmap(fdf);
	if (!(fdf->mlx.mlx = mlx_init()))
		ft_getout("Failed to connect into X server");
	if (!(fdf->mlx.win = mlx_new_window(fdf->mlx.mlx, WIN_WIDTH, WIN_HEIGHT,
	"fdf@eniini")))
		ft_getout("X server failed to create a window");
	fdf->img.img = mlx_new_image(fdf->mlx.mlx, WIN_WIDTH, WIN_HEIGHT);
	fdf->img.addr = mlx_get_data_addr(fdf->img.img, &fdf->img.bpp,
	&fdf->img.l_len, &fdf->img.endian);
	return (fdf);
}

/*
**	Checks that program is run with an argument, then initializes fdf.
**
**	deal_key() hook is called with x11 event that equates to KeyPress
**	(instead of mlx_key_hook that calls KeyRelease) with its corresponding
**	mask to enable smooth user input.
**
**	Hooks draw() into the mlx_loop, which lets the program run until
**	it receives the appropriate exit signal.
*/

int		main(int argc, char **argv)
{
	t_fdf	*fdf;

	if (argc != 2)
		print_usage();
	fdf = init(argv[1]);
	mlx_hook(fdf->mlx.win, 2, (1L << 0), deal_key, fdf);
	mlx_loop_hook(fdf->mlx.mlx, draw_logic, fdf);
	mlx_loop(fdf->mlx.mlx);
	return (0);
}
