/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 15:26:08 by eniini            #+#    #+#             */
/*   Updated: 2021/01/19 17:51:42 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Handles drawing all the fdf elements into the mlx window.
**	Loops indefinitely until the exit-event is called.
*/

#include "fdf.h"

/*
**	Clears [fdf->img], then fills the screen with background color(s).
**	Always called before grid & UI drawing.
*/

static void	draw_bg(t_fdf *fdf)
{
	int		*image;
	int		i;

	ft_bzero(fdf->img.addr, WIN_WIDTH * WIN_HEIGHT * (fdf->img.bpp / 8));
	image = (int *)(fdf->img.addr);
	i = 0;
	while (i < WIN_HEIGHT * WIN_WIDTH)
		image[i++] = 0x000000;
}

/*
**	[vrt == 1] draws vertical grid lines, [0] horizontal ones.
**	Generates color for both grid points according to each z-value.
**	Applies 1) translation (position and size of the grid to be drawn)
**			2) scaling (projection type, drawing distance, map depth)
**			3) rotation (X/Y/Z) to the grid.
**	[p.z] is used to hold the map depth values for the rotation matrix
**	calculations since drawline() uses only the transformed [x/y] values.
*/

static void	draw_perspective(t_fdf *fdf, int x, int y, int vrt)
{
	int		xt;
	int		yt;

	fdf->p0.c = get_color(fdf, fdf->map.z_arr[x][y]);
	fdf->p1.c = vrt == 1 ? get_color(fdf, fdf->map.z_arr[x + 1][y]) :
	get_color(fdf, fdf->map.z_arr[x][y + 1]);
	xt = x - fdf->map.height / 2;
	yt = y - fdf->map.width / 2;
	fdf->p0.x = fdf->map.angle_z * (yt + xt) * fdf->map.zoom;
	fdf->p0.y = fdf->map.angle_y * (yt - xt) * fdf->map.zoom;
	fdf->p1.x = vrt == 1 ? fdf->map.angle_z * (yt + (xt + 1)) * fdf->map.zoom :
	fdf->map.angle_z * ((yt + 1) + xt) * fdf->map.zoom;
	fdf->p1.y = vrt == 1 ? fdf->map.angle_y * (yt - (xt + 1)) * fdf->map.zoom :
	fdf->map.angle_y * ((yt + 1) - xt) * fdf->map.zoom;
	fdf->p0.z = fdf->map.z_arr[x][y] * (fdf->map.z_val * 0.25);
	fdf->p1.z = vrt == 1 ? fdf->map.z_arr[x + 1][y] * (fdf->map.z_val * 0.25) :
	fdf->map.z_arr[x][y + 1] * (fdf->map.z_val * 0.25);
	fdf->p0.x -= fdf->p0.z;
	fdf->p1.x -= fdf->p1.z;
	rotations(shader(fdf));
	fdf->p0.x += (WIN_HEIGHT / 2) + fdf->map.o_x;
	fdf->p1.x += (WIN_HEIGHT / 2) + fdf->map.o_x;
	fdf->p0.y += (WIN_WIDTH / 2) + fdf->map.o_y;
	fdf->p1.y += (WIN_WIDTH / 2) + fdf->map.o_y;
	drawline(fdf);
}

/*
**	Draws the grid orthographically on it's 'side' with bottom row in front.
**	Therefore up/down-axis represents grids depth values.
**	Finally, the grid is shaded from back to front to give a sense of depth.
*/

static void	draw_ortho(t_fdf *fdf, int x, int y, int vrt)
{
	fdf->p0.c = get_color(fdf, fdf->map.z_arr[x][y]);
	fdf->p1.c = vrt == 1 ? get_color(fdf, fdf->map.z_arr[x + 1][y]) :
	get_color(fdf, fdf->map.z_arr[x][y + 1]);
	fdf->p0.x = fdf->map.zoom - fdf->map.width / 2;
	fdf->p1.x = fdf->map.zoom - fdf->map.width / 2;
	fdf->p0.y = (y - fdf->map.width / 2) * fdf->map.zoom;
	fdf->p1.y = vrt == 1 ? (y - fdf->map.width / 2) * fdf->map.zoom :
	(y + 1 - fdf->map.width / 2) * fdf->map.zoom;
	fdf->p0.x -= fdf->map.z_arr[x][y] * fdf->map.z_val;
	fdf->p1.x -= vrt == 1 ? fdf->map.z_arr[x + 1][y] * fdf->map.z_val :
	fdf->map.z_arr[x][y + 1] * fdf->map.z_val;
	fdf->p0.x += (WIN_HEIGHT / 2) + fdf->map.o_x;
	fdf->p1.x += (WIN_HEIGHT / 2) + fdf->map.o_x;
	fdf->p0.y += (WIN_WIDTH / 2) + fdf->map.o_y;
	fdf->p1.y += (WIN_WIDTH / 2) + fdf->map.o_y;
	fdf->p0.z = fdf->map.z_arr[x][y] * (fdf->map.z_val * 0.25);
	fdf->p1.z = vrt == 1 ? fdf->map.z_arr[x + 1][y] * (fdf->map.z_val * 0.25) :
	fdf->map.z_arr[x][y + 1] * (fdf->map.z_val * 0.25);
	shader_ortho(fdf, x, vrt);
	drawline(fdf);
}

/*
**	Draw the contents of [str] into [mlx->img].
*/

static void	draw_ui(t_fdf *fdf)
{
	char	*str;

	str = "[Esc]     quit program             [R]       reset grid";
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 20, 0xFFFFFF, str);
	str = "[Space]   switch camera type       [X/Y/Z]     rotate grid";
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 35, 0xFFFFFF, str);
	str = "[N/M]     change elevation         [T]       toggle autorotation";
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 50, 0xFFFFFF, str);
	str = "[Q/E]     zoom                     [WASD]    move grid";
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 65, 0xFFFFFF, str);
	str = "[9 & 0]   save and load current fdf state";
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 80, 0xFFFFFF, str);
	str = "[H]       hold to shift the hue of grid colors";
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 95, 0xFFFFFF, str);
	str = "[J]       switch depth-based shading on/off";
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 110, 0xFFFFFF, str);
	str = "[K]       create a bitmap snapshot";
	mlx_string_put(fdf->mlx.mlx, fdf->mlx.win, 20, 125, 0xFFFFFF, str);
}

/*
**	Calls the drawing routines to draw a background, [fdf->map] and
**	UI on to [fdf->img.img].
**	Not using a loop of destroying/re-creating mlx images as that gave
**	inconsistent results on VM/Unix. Instead, all elements are drawn on
**	the image which is then pushed and reset on a new loop.
*/

int			draw_logic(t_fdf *fdf)
{
	int		x;
	int		y;

	draw_bg(fdf);
	x = 0;
	while (x < fdf->map.height)
	{
		y = 0;
		while (y < fdf->map.width)
		{
			if (y + 1 < fdf->map.width)
				fdf->map.proj == 1 ? draw_perspective(fdf, x, y, 0) :
				draw_ortho(fdf, x, y, 0);
			if (x + 1 < fdf->map.height)
				fdf->map.proj == 1 ? draw_perspective(fdf, x, y, 1) :
				draw_ortho(fdf, x, y, 1);
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(fdf->mlx.mlx, fdf->mlx.win, fdf->img.img, 0, 0);
	draw_ui(fdf);
	if (fdf->loop == 1)
		rotation_debug(fdf);
	return (0);
}
