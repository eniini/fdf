/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eniini <eniini@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/12 18:17:22 by eniini            #+#    #+#             */
/*   Updated: 2021/01/19 17:58:44 by eniini           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	Reads and stores relevant data from the elevation map given as argument
**	for the program.
**	Maps are required to have an even count of individual values per line.
**	Maps with non-decimal values can give inaccurate results as all read data
**	is converted to integers.
*/

#include "fdf.h"

/*
**	Reads from arg, checks for potential errors and counts the n of numbers
**	(batches of symbols, everything is converted to int afterwards) per line.
**	Width of the first line is saved and subsequent lines are compared against
**	it to ensure that the map is square-shaped.
*/

static int	check_lines(t_fdf *fdf, char *argv)
{
	int		fd;
	int		i;
	int		cols;
	char	*line;

	i = 0;
	cols = 0;
	if ((fd = open(argv, O_RDONLY)) < 0)
		ft_getout(strerror(errno));
	while (get_next_line(fd, &line) == 1)
	{
		if ((fdf->map.width = (int)ft_wordcount(line, ' ')) > cols)
			cols = fdf->map.width;
		(cols == fdf->map.width) ? i++ : ft_getout("Invalid map width");
		free(line);
	}
	if (close(fd))
		ft_getout(strerror(errno));
	if (!(fdf->map.width = cols))
		ft_getout("Invalid file type");
	return (i);
}

/*
**	Reads arg again, this time converting map into usable format.
**	Highest and lowest z-values are stored into [fdf->map.d(epth)].
*/

static void	get_depth(t_fdf *fdf, int x, char *line)
{
	char		**values;
	int			i;
	int			y;

	y = 0;
	if (!(values = ft_strsplit(line, ' ')))
		ft_getout(strerror(errno));
	else
	{
		i = 0;
		while (values[i] && y != fdf->map.width)
		{
			fdf->map.z_arr[x][y] = ft_atoi(values[i]);
			if (fdf->map.d_max < fdf->map.z_arr[x][y])
				fdf->map.d_max = fdf->map.z_arr[x][y];
			if (fdf->map.d_min > fdf->map.z_arr[x][y])
				fdf->map.d_min = fdf->map.z_arr[x][y];
			y++;
			i++;
		}
	}
	ft_free_arr(values);
}

/*
**	[Fdf->map.d] limits are initialized to non-zero values to avoid undefined
**	behavior.
**	No need to 0-terminate [z_arr] since its size is known as [map->height].
*/

void		read_map(char *argv, t_fdf *fdf)
{
	char	*line;
	int		fd;
	int		x;

	fdf->map.d_max = 1;
	fdf->map.d_min = -1;
	x = 0;
	fdf->map.height = check_lines(fdf, argv);
	if ((fd = open(argv, O_RDONLY)) < 0)
		ft_getout(strerror(errno));
	if (!(fdf->map.z_arr = (int**)malloc(sizeof(int *) * fdf->map.height)))
		ft_getout(strerror(errno));
	while (get_next_line(fd, &line) == 1 && x != fdf->map.height)
	{
		if (!(fdf->map.z_arr[x] =
		(int *)malloc(sizeof(int) * fdf->map.width)))
			ft_getout(strerror(errno));
		get_depth(fdf, x++, line);
		free(line);
	}
	if (close(fd))
		ft_getout(strerror(errno));
}
