/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotations.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 23:47:51 by eniini            #+#    #+#             */
/*   Updated: 2021/01/14 23:50:07 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
**	Applies rotation matrix transformations into [fdf->map.p] instances
**	where θ is corresponding [fdf->map.(axis)_r] value converted to radians.
**
**	X-axis		{y' =  y * cos(θ) + z * sin(θ)}
**	rotation	{z' = -y * sin(θ) + z * cos(θ)}
**
**	Y-axis		{x' =  x * cos(θ) + z * sin(θ)}
**	rotation	{z' = -x * sin(θ) + z * cos(θ)}
**
**	Z-axis		{x' =  x * cos(θ) - y * sin(θ)}
**	rotation	{y' =  x * sin(θ) + y * cos(θ)}
*/

static void	rotate_x(int *z, int *y, int r)
{
	int	prev_y;

	prev_y = *y;
	*y = prev_y * cos(r * RAD_CON) + *z * sin(r * RAD_CON);
	*z = -prev_y * sin(r * RAD_CON) + *z * cos(r * RAD_CON);
}

static void	rotate_y(int *x, int *z, int r)
{
	int	prev_x;

	prev_x = *x;
	*x = prev_x * cos(r * RAD_CON) + *z * sin(r * RAD_CON);
	*z = -prev_x * sin(r * RAD_CON) + *z * cos(r * RAD_CON);
}

static void	rotate_z(int *x, int *y, int r)
{
	int	prev_x;
	int	prev_y;

	prev_x = *x;
	prev_y = *y;
	*x = prev_x * cos(r * RAD_CON) - prev_y * sin(r * RAD_CON);
	*y = prev_x * sin(r * RAD_CON) + prev_y * cos(r * RAD_CON);
}

void		rotations(t_fdf *fdf)
{
	rotate_x(&fdf->p0.z, &fdf->p0.y, fdf->map.x_r);
	rotate_y(&fdf->p0.x, &fdf->p0.z, fdf->map.y_r);
	rotate_z(&fdf->p0.x, &fdf->p0.y, fdf->map.z_r);
	rotate_x(&fdf->p1.z, &fdf->p1.y, fdf->map.x_r);
	rotate_y(&fdf->p1.x, &fdf->p1.z, fdf->map.y_r);
	rotate_z(&fdf->p1.x, &fdf->p1.y, fdf->map.z_r);
}
