/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/06 13:54:55 by eniini            #+#    #+#             */
/*   Updated: 2021/01/18 20:05:19 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

/*
**	Standard libraries to access math functions, open/read/close/exit() and
**	stderror() + errnum.
**	[Sys/stat] included for Fdf's state saving feature.
*/

# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/stat.h>

# include "libft.h"
# include "mlx.h"
# include "keys.h"

# define WIN_HEIGHT	900
# define WIN_WIDTH	1600

# ifndef M_PI
#  define M_PI		3.14159265358979323846
# endif

# define DEF_HIGH	0x20D6AC
# define DEF_MID	0x0648BA
# define DEF_LOW	0x73276F
# define DEBUG		0xF44444

/*
** Converts from degree into radians (π / 180)
*/

# define RAD_CON	0.01745329251993888928

typedef struct	s_point
{
	int			x;
	int			y;
	int			z;
	int			c;
}				t_point;

typedef struct	s_mlx
{
	void		*mlx;
	void		*win;
}				t_mlx;

typedef struct	s_img
{
	void		*img;
	char		*addr;
	int			bpp;
	int			l_len;
	int			endian;
}				t_img;

typedef struct	s_map
{
	int			**z_arr;
	int			height;
	int			width;
	int			zoom;
	int			o_x;
	int			o_y;
	int			z_val;
	double		angle_y;
	double		angle_z;
	int			proj;
	int			x_r;
	int			y_r;
	int			z_r;
	int			d_min;
	int			d_max;
}				t_map;

typedef struct	s_col
{
	int			shaded;
	int			high;
	int			mid;
	int			low;
}				t_col;

typedef struct	s_fdf
{
	t_point		p0;
	t_point		p1;
	t_map		map;
	t_mlx		mlx;
	t_img		img;
	t_col		col;
	int			loop;
}				t_fdf;

typedef struct	s_rgb_mat
{
	float		rix[3][3];
	float		cos_a;
	float		sin_a;
	double		axis_unit;
}				t_rgb_mat;

void			resetmap(t_fdf *fdf);

void			print_usage(void);

void			rotation_debug(t_fdf *fdf);

int				deal_key(int key, t_fdf *fdf);

void			read_map(char *argv, t_fdf *fdf);

int				draw_logic(t_fdf *fdf);

void			drawline(t_fdf *fdf);

void			rotations(t_fdf *fdf);

int				color_lerp(int c1, int c2, double p);

int				get_color(t_fdf *fdf, int z);

t_fdf			*shader(t_fdf *fdf);

void			shader_ortho(t_fdf *fdf, int x, int vrt);

int				shift_hue(int c, double p);

void			save_state(t_fdf *fdf, int fd);

void			load_state(t_fdf *fdf, int fd);

void			create_bmp(t_fdf *fdf, char *f);

#endif
