/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 15:26:44 by eniini            #+#    #+#             */
/*   Updated: 2021/01/18 20:36:05 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	ARGB data manipulation and initialization functions.
**	Bit operations are done in little-endian, as the program is meant to be
**	run in unix/macOS.
*/

#include "fdf.h"

/*
**	Interpolates individual RGB values and returns them as one ARGB integer.
*/

int			color_lerp(int c1, int c2, double p)
{
	int			r;
	int			g;
	int			b;

	if (c1 == c2)
		return (c1);
	r = ft_i_lerp((c1 >> 16) & 255, (c2 >> 16) & 255, p);
	g = ft_i_lerp((c1 >> 8) & 255, (c2 >> 8) & 255, p);
	b = ft_i_lerp(c1 & 255, c2 & 255, p);
	return (0 << 24 | r << 16 | g << 8 | b);
}

/*
**	Returns interpolated shade of color of the value [z] between
**	either maximum or minimum value found in the [fdf->map] and (0).
*/

int			get_color(t_fdf *fdf, int z)
{
	if (z < 0)
	{
		return (color_lerp(fdf->col.mid, fdf->col.low, ft_abs(z) /
		(double)ft_abs(fdf->map.d_min)));
	}
	return (color_lerp(fdf->col.mid, fdf->col.high, z /
	(double)fdf->map.d_max));
}

/*
**	Interpolates given color towards pure black or white depending
**	on its z-value.
**	Returns a pointer to fdf to save space in draw_grid() function.
*/

t_fdf		*shader(t_fdf *fdf)
{
	int			z0;
	int			z1;

	if (fdf->col.shaded == 0)
		return (fdf);
	z0 = ft_clamp_i(ft_abs(fdf->p0.z), 0, 255);
	z1 = ft_clamp_i(ft_abs(fdf->p1.z), 0, 255);
	if (fdf->p0.z < 0)
		fdf->p0.c = color_lerp(fdf->p0.c, 0x000000, z0 / 255.);
	else
		fdf->p0.c = color_lerp(fdf->p0.c, 0xFFFFFF, z0 / 255.);
	if (fdf->p1.z < 0)
		fdf->p1.c = color_lerp(fdf->p1.c, 0x000000, z1 / 255.);
	else
		fdf->p1.c = color_lerp(fdf->p1.c, 0xFFFFFF, z1 / 255.);
	return (fdf);
}

/*
**	Applies shade to the orthographic view, dimming grid layers
**	farthest from the bottom/front row to give a sense of depth.
**	Additionally performs depth-based shading if its toggled on.
*/

void		shader_ortho(t_fdf *fdf, int x, int vrt)
{
	int			z0;
	int			z1;

	if (fdf->col.shaded)
	{
		z0 = ft_clamp_i(ft_abs(fdf->p0.z), 0, 255);
		z1 = ft_clamp_i(ft_abs(fdf->p1.z), 0, 255);
		if (fdf->p0.z < 0)
			fdf->p0.c = color_lerp(fdf->p0.c, 0x000000, z0 / 255.);
		else
			fdf->p0.c = color_lerp(fdf->p0.c, 0xFFFFFF, z0 / 255.);
		if (fdf->p1.z < 0)
			fdf->p1.c = color_lerp(fdf->p1.c, 0x000000, z1 / 255.);
		else
			fdf->p1.c = color_lerp(fdf->p1.c, 0xFFFFFF, z1 / 255.);
	}
	z0 = ft_clamp_i(x, 0, fdf->map.height / 2);
	z1 = vrt == 1 ? ft_clamp_i(x + 1, 0, fdf->map.height / 2) :
	ft_clamp_i(x, 0, fdf->map.height / 2);
	fdf->p0.c = color_lerp(fdf->p0.c, 0x000000, z0 / (double)fdf->map.height);
	fdf->p1.c = color_lerp(fdf->p1.c, 0x000000, z1 / (double)fdf->map.height);
}

/*
**	Since RGB values effectively form a cube of color space, we're able to
**	shift a hue of a 'color point' by applying an axis+angle rotation matrix.
**	[axis_unit] is a simplified constant for axis vector calculations.
**
**	Since mlx color is stored as int, final calculation of values is rounded up
**	to preserve original brightness level.
**
**	Return values are clamped since diagonal of RGB cube and its rotation
**	result in values outside a 255³.
*/

int			shift_hue(int c, double p)
{
	t_rgb_mat	mat;
	int			rx;
	int			gx;
	int			bx;

	mat.cos_a = cos(p * RAD_CON);
	mat.sin_a = sin(p * RAD_CON);
	mat.axis_unit = sqrt(1. / 3.);
	mat.rix[0][0] = mat.cos_a + (1.0 - mat.cos_a) / 3.0;
	mat.rix[0][1] = 1. / 3. * (1.0 - mat.cos_a) - mat.axis_unit * mat.sin_a;
	mat.rix[0][2] = 1. / 3. * (1.0 - mat.cos_a) + mat.axis_unit * mat.sin_a;
	mat.rix[1][0] = 1. / 3. * (1.0 - mat.cos_a) + mat.axis_unit * mat.sin_a;
	mat.rix[1][1] = mat.cos_a + 1. / 3. * (1.0 - mat.cos_a);
	mat.rix[1][2] = 1. / 3. * (1.0 - mat.cos_a) - mat.axis_unit * mat.sin_a;
	mat.rix[2][0] = 1. / 3. * (1.0 - mat.cos_a) - mat.axis_unit * mat.sin_a;
	mat.rix[2][1] = 1. / 3. * (1.0 - mat.cos_a) + mat.axis_unit * mat.sin_a;
	mat.rix[2][2] = mat.cos_a + 1. / 3. * (1.0 - mat.cos_a);
	rx = round(((c >> 16) & 255) * mat.rix[0][0] + ((c >> 8) & 255)
	* mat.rix[0][1] + (c & 255) * mat.rix[0][2]);
	gx = round(((c >> 16) & 255) * mat.rix[1][0] + ((c >> 8) & 255)
	* mat.rix[1][1] + (c & 255) * mat.rix[1][2]);
	bx = round(((c >> 16) & 255) * mat.rix[2][0] + ((c >> 8) & 255)
	* mat.rix[2][1] + (c & 255) * mat.rix[2][2]);
	return (0 << 24 | ft_clamp_i(rx, 0, 255) << 16 |
	ft_clamp_i(gx, 0, 255) << 8 | ft_clamp_i(bx, 0, 255));
}
