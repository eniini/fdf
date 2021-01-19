/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 15:21:01 by eniini            #+#    #+#             */
/*   Updated: 2021/01/15 21:58:00 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Includes the implementation of Bresenham's line algorithm, function
**	to manipulate individual pixels in the one-dimensional array initialized
**	by mlx into [fdf->img.addr] and perspective projection transformations.
*/

#include "fdf.h"

/*
**	[Dst] is a temporary pointer that's initialized to point at the
**	x/y coordinate in the created image. The pointer is then typecasted to
**	parameter [color].
*/

static void	my_mlx_pixel_put(t_fdf *fdf, int x, int y, int color)
{
	char		*dst;

	if ((y > 0 && x > 0) && (y < WIN_HEIGHT && x < WIN_WIDTH))
	{
		dst = fdf->img.addr + (y * fdf->img.l_len + x * (fdf->img.bpp / 8));
		*(unsigned int*)dst = color;
	}
}

/*
**	Line drawing algorithm between two point coordinates stored in [fdf->map].
**	Drawline() makes sure that drawing works for all angles into
**	every direction, then calls draw_drawline() that plots
**	a line of pixels from [map.point0] to [map.point1].
*/

static void	draw_drawline(t_fdf *fdf, int x, int y, int flip)
{
	int			derror;
	int			error;
	double		p;
	int			p_color;

	derror = ft_abs(fdf->p1.y - fdf->p0.y) * 2;
	error = 0;
	while (x <= fdf->p1.x)
	{
		p = (fdf->p1.x - fdf->p0.x) > (fdf->p1.y - fdf->p0.y) ?
		ft_inverse_i_lerp(fdf->p0.x, fdf->p1.x, x) :
		ft_inverse_i_lerp(fdf->p0.y, fdf->p1.y, y);
		p_color = color_lerp(fdf->p0.c, fdf->p1.c, p);
		if (flip)
			my_mlx_pixel_put(fdf, x, y, p_color);
		else
			my_mlx_pixel_put(fdf, y, x, p_color);
		error += derror;
		if (error > fdf->p1.x - fdf->p0.x)
		{
			y += (fdf->p1.y > fdf->p0.y) ? 1 : -1;
			error -= (fdf->p1.x - fdf->p0.x) * 2;
		}
		x++;
	}
}

void		drawline(t_fdf *fdf)
{
	int			flip;

	if (ft_abs(fdf->p0.x - fdf->p1.x) <
	ft_abs(fdf->p0.y - fdf->p1.y))
	{
		ft_swap(&fdf->p0.x, &fdf->p0.y, sizeof(int));
		ft_swap(&fdf->p1.x, &fdf->p1.y, sizeof(int));
		flip = 1;
	}
	else
		flip = 0;
	if (fdf->p0.x > fdf->p1.x)
	{
		ft_swap(&fdf->p0.x, &fdf->p1.x, sizeof(int));
		ft_swap(&fdf->p0.y, &fdf->p1.y, sizeof(int));
		ft_swap(&fdf->p0.c, &fdf->p1.c, sizeof(int));
	}
	draw_drawline(fdf, fdf->p0.x, fdf->p0.y, flip);
}
