/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 15:54:14 by eniini            #+#    #+#             */
/*   Updated: 2021/01/19 17:51:24 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Created to handle X11 events corresponding to macros defined in [keys.h].
**
**	Since only deal_key() is hooked to listen for keypresses in main(),
**	functions deal_key_graphics() & deal_key_savestate() are always called
**	in tandem by it to circulate the keypress information.
*/

#include "fdf.h"

/*
**	Save_state() function call is preceded by two open checks: First will check
**	if [savestate] file exists (to be overwritten) and the second checks if
**	it's possible to be created.
**
**	Load_state() only checks if the savefile exists (it's assumed to be one
**	created by this version of fdf!)
*/

static int	deal_key_savestate(int key, t_fdf *fdf)
{
	int		fd;

	if (key == KEY_9)
	{
		if ((fd = open(".savestate", O_WRONLY | O_TRUNC | O_APPEND)) < 0)
		{
			if ((fd = open(".savestate", O_WRONLY | O_CREAT | O_EXCL |
			O_APPEND | S_IRWXU)) < 0)
				ft_getout(strerror(errno));
		}
		save_state(fdf, fd);
	}
	else if (key == KEY_0)
	{
		if ((fd = open(".savestate", O_RDONLY)) < 0)
			ft_putendl("failed to load fdf state!");
		else
			load_state(fdf, fd);
	}
	return (0);
}

/*
**	[z_val] indicates the magnitude of depth differences in the wireframe.
**	[proj == 1] applies perspective projection, (0) for isometric.
*/

static int	deal_key_graphics(int key, t_fdf *fdf)
{
	if (key == KEY_J)
		fdf->col.shaded = (fdf->col.shaded % 2) ? 0 : 1;
	else if ((key == KEY_M) && (fdf->map.z_val < 100))
		fdf->map.z_val += 1;
	else if ((key == KEY_N) && (fdf->map.z_val > -100))
		fdf->map.z_val -= 1;
	else if (key == KEY_SPACE)
		fdf->map.proj = (fdf->map.proj % 2) ? 0 : 1;
	else if (key == KEY_K)
	{
		create_bmp(fdf, "fdf.bmp");
		ft_putendl("bitmap image created!");
	}
	if (key == KEY_H)
	{
		fdf->col.high = shift_hue(fdf->col.high, 5.);
		fdf->col.mid = shift_hue(fdf->col.mid, 5.);
		fdf->col.low = shift_hue(fdf->col.low, 5.);
	}
	return (deal_key_savestate(key, fdf));
}

/*
**	[WASD] control where the grid is drawn, and its orientation is translated
**	by how close the grid is to the camera.
**	[X/Y] control grid rotation around its origin.
*/

int			deal_key(int key, t_fdf *fdf)
{
	if (key == KEY_ESC)
		exit(EXIT_SUCCESS);
	else if (key == KEY_R)
		resetmap(fdf);
	else if (key == KEY_Q && fdf->map.zoom > 1)
		fdf->map.zoom -= 1;
	else if (key == KEY_E)
		fdf->map.zoom += 1;
	else if (key == KEY_A)
		fdf->map.o_y += 1 * fdf->map.zoom;
	else if (key == KEY_D)
		fdf->map.o_y -= 1 * fdf->map.zoom;
	else if (key == KEY_W)
		fdf->map.o_x += 1 * fdf->map.zoom;
	else if (key == KEY_S)
		fdf->map.o_x -= 1 * fdf->map.zoom;
	else if (key == KEY_X)
		fdf->map.x_r++;
	else if (key == KEY_Y)
		fdf->map.y_r++;
	else if (key == KEY_Z)
		fdf->map.z_r++;
	else if (key == KEY_T)
		fdf->loop = (fdf->loop == 1) ? 0 : 1;
	return (deal_key_graphics(key, fdf));
}

/*
**	Prints instructions if no argument is given.
*/

void		print_usage(void)
{
	ft_putendl("Usage:\n\tfdf example_map.fdf");
	ft_putstr("Controls:\n");
	ft_putendl("\t[Esc] to quit\t\t\t[R] to reset grid");
	ft_putendl("\t[Space] to switch camera type\t[X/Y/Z] to rotate grid");
	ft_putendl("\t[N/M] to change elevation\t[T] toggle autorotation");
	ft_putendl("\t[Q/E] to zoom\t\t\t[WASD] to move grid");
	ft_putendl("\t[9] to save current grid state");
	ft_putendl("\t[0] to load previously saved data");
	ft_putendl("\t[H] hold to shift the hue of grid colors");
	ft_putendl("\t[J] toggle depth-based shading");
	ft_putendl("\t[K] to take bitmap snapshot");
	exit(EXIT_FAILURE);
}

/*
**	Applies an automatic X & Y-axis rotation onto the fdf grid
**	while displaying both values for debugging purposes.
**	On/off-toggle functionality.
*/

void		rotation_debug(t_fdf *fdf)
{
	char		*str;

	if (fdf->map.x_r < 360)
		fdf->map.x_r++;
	if (fdf->map.x_r == 360)
		fdf->map.x_r = 0;
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 200, DEBUG, "[X]  [Y]  [Z]");
	str = ft_itoa(fdf->map.x_r);
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 215, DEBUG, str);
	free(str);
	str = ft_itoa(fdf->map.y_r);
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 50, 215, DEBUG, str);
	free(str);
	str = ft_itoa(fdf->map.z_r);
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 80, 215, DEBUG, str);
	free(str);
}
